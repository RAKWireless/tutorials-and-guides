#include "metadata.h"
#include <Arduino.h>
#include <FIR.h>
#include <Notecard.h>
#include <Wire.h>
#include <floor_tracker.h>
#include "WB_press_sensor.h"
#include <LoRaWan-RAK4630.h> //http://librarymanager/All#SX126x
#include <SPI.h>

// Uncomment to use a connected SSD1306 Display
// #define USE_DISPLAY

// Uncomment to rotate the connected display 180 degrees
// #define DISPLAY_ROTATE_180

// Uncomment to give the OLED Display power from digital MCU pins.
// Safe for 0.91 Inch I2C OLED Display on Notecarrier-F + Blues Swan.
// UNSAFE if your display takes more amps than your microcontroller can supply.
// #define DISPLAY_POWER_PIN 5
// #define DISPLAY_GROUND_PIN 6

// Uncomment to output the floor sample every second
// #define DEBUG_FLOOR_SAMPLES

#ifdef USE_DISPLAY
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#endif

// Uncomment to view Note requests from the Host
// #define DEBUG_NOTECARD

// This is the unique Product Identifier for your device
#ifndef PRODUCT_UID
#define PRODUCT_UID "com.my-company.my-name:my-project" // "com.my-company.my-name:my-project"
#pragma message "PRODUCT_UID is not defined in this example. Please ensure your Notecard has a product identifier set before running this example or define it in code here. More details at https://dev.blues.io/notehub/notehub-walkthrough/#finding-a-productuid"
#endif

#define ENV_POLL_SECS 1
#define IDLE_UPDATE_PERIOD (1000 * 60 * 5)
#define LIVE_UPDATE_PERIOD (1000 * 60 * 1)
#define NO_MOVEMENT_THRESHOLD_SCALE_MS \
  (1000) // No movement threshold given in seconds.
#define FLOOR_SAMPLE_PERIOD (250)
#define FLOOR_FILTER_ORDER (10)
#define FLOOR_OFFSET (0.3)

// Variables for Env Var polling
static unsigned long nextPollMs = 0;
static uint32_t lastModifiedTime = 0;
// Variables for sensor reading period when not in live mode
static unsigned long lastUpdateMs;
static unsigned long updatePeriod;
static unsigned long lastFloorSampleMs;

FIR<double, FLOOR_FILTER_ORDER> floorFilter;

bool setBaselineFloor = false;
applicationState state = {0};

Notecard notecard;

// Forward declarations
void fetchEnvironmentVariables(applicationState &state);
bool pollEnvVars(void);
sensorReadings captureSensorReadings(void);
void sendSensorReadings(const sensorReadings &readings, bool alarm = false);
void displayReadings(const sensorReadings &readings);
bool publishSensorReadings(sensorReadings &readings, uint32_t currentMillis);

#ifdef USE_DISPLAY
Adafruit_SSD1306 display = Adafruit_SSD1306(128, 32, &Wire);
#endif

void setup()
{
  conf_bar();
  
#ifdef DEBUG_NOTECARD
  notecard.setDebugOutputStream(Serial);
#endif
  delay(1500);
  Serial.println("Floor Level Detector");
  Serial.println("====================");

  notecard.begin();
  Serial.println("Tarjeta iniciada");

#ifdef USE_DISPLAY
#ifdef DISPLAY_POWER_PIN
  pinMode(DISPLAY_POWER_PIN, OUTPUT);
  digitalWrite(DISPLAY_POWER_PIN, HIGH);
#endif
#ifdef DISPLAY_GROUND_PIN
  pinMode(DISPLAY_GROUND_PIN, OUTPUT);
  digitalWrite(DISPLAY_GROUND_PIN, LOW);
#endif
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  Serial.println("OLED connected...");
#endif

  double coef[FLOOR_FILTER_ORDER];
  for (int i = 0; i < FLOOR_FILTER_ORDER; i++)
  {
    coef[i] = 1.0; // rolling average
  }
  floorFilter.setFilterCoeffs(coef);

#ifdef USE_DISPLAY
  // Clear the display buffer.
  display.clearDisplay();
  display.display();

  // text display tests
#ifdef DISPLAY_ROTATE_180
  display.setRotation(2);
#endif
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.println("NF1");
  display.println("Indoor Floor Level Tracker");
  display.setCursor(0, 0);
  display.display();
#endif

  J *req = notecard.newRequest("hub.set");
  if (req != NULL)
  {
    JAddStringToObject(req, "product", PRODUCT_UID);
    JAddStringToObject(req, "mode", "continuous");
    JAddBoolToObject(req, "sync", true);
    notecard.sendRequest(req);
  }

  // Notify Notehub of the current firmware version
  req = notecard.newRequest("dfu.status");
  if (req != NULL)
  {
    JAddStringToObject(req, "version", firmwareVersion());
    notecard.sendRequest(req);
  }

  // Enable Outboard DFU
  req = notecard.newRequest("card.dfu");
  if (req != NULL)
  {
    JAddStringToObject(req, "name", "wisblock");
    JAddBoolToObject(req, "on", true);
    notecard.sendRequest(req);
  }
  Serial.println("Tarjeta Configurada");

  // Check Environment Variables
  fetchEnvironmentVariables(state);

  delay(2000);

  if (!state.live)
  {
    if (state.floorHeight != 0.0 && state.baselineFloor != 0)
    {
      sensorReadings readings = captureSensorReadings();
      displayReadings(readings);
      sendSensorReadings(readings);
    }
    else
    {
      Serial.println(
          "Waiting for Environment Variables from the Notecard");
    }
  }
  lastUpdateMs = millis();
  Serial.println("Setup Ready!");
}

void resetFloorFilter(double floor)
{
  for (int i = 0; i < FLOOR_FILTER_ORDER; i++)
  {
    floorFilter.processReading(floor);
  }
}

void loop()
{
  if (pollEnvVars())
  {
    fetchEnvironmentVariables(state);

    if (state.variablesUpdated)
    {
      Serial.println("Environment Variable Updates Received");

      J *req = notecard.newRequest("note.add");
      if (req != NULL)
      {
        JAddStringToObject(req, "file", "notify.qo");
        JAddBoolToObject(req, "sync", true);
        J *body = JCreateObject();
        if (body != NULL)
        {
          JAddStringToObject(body, "message",
                             "environment variable update received");
          JAddItemToObject(req, "body", body);
          notecard.sendRequest(req);
        }
      }
      state.variablesUpdated = false;
    }
    
  }

  const uint32_t currentMillis = millis();
  if (!state.live)
  {
    if (currentMillis - lastUpdateMs >= updatePeriod)
    {
      if (state.floorHeight != 0.0 && state.baselineFloor != 0)
      {
        sensorReadings readings = captureSensorReadings();
        displayReadings(readings);
        lastUpdateMs = currentMillis;
        sendSensorReadings(readings);
      }
      else
      {
        Serial.println(
            "Waiting for Environment Variables from the Notecard");
      }
    }
  }
  else
  {
    if (state.floorHeight != 0.0 && state.baselineFloor != 0)
    {
      sensorReadings readings = captureSensorReadings();
      if ((currentMillis - lastFloorSampleMs) >= FLOOR_SAMPLE_PERIOD)
      {
        lastFloorSampleMs = currentMillis;
        publishSensorReadings(readings, currentMillis);
      }
    }
    else
    {
      Serial.println(
          "Waiting for Environment Variables from the Notecard");
    }
  }
  
}

bool publishSensorReadings(sensorReadings &readings, uint32_t currentMillis)
{
  bool send = false;
  bool alarm = false;
  bool floorChange = false;

  // run the floor signal through a FIR low-pass filter
  double filteredFloor = floorFilter.processReading(readings.floor);
  readings.currentFloor = (int)(filteredFloor + FLOOR_OFFSET);

#ifdef DEBUG_FLOOR_SAMPLES
  Serial.print("floor sample: ");
  Serial.print(readings.floor);
  Serial.print(", filtered: ");
  Serial.print(filteredFloor);
  Serial.println();
#endif

  if (state.baselineChanged)
  {
    send = true;
    floorChange = true;
    state.alarmSent = false;
    state.baselineChanged = false;
    Serial.println("Baseline changed. Sending a Note.");
  }
  else if (readings.currentFloor != state.lastFloor)
  {
    send = true;
    floorChange = true;
    state.alarmSent = false;
    Serial.println("New floor detected. Sending a Note.");
  }
  else if (!state.alarmSent && state.lastFloorChangeAt &&
           state.lastFloorChangeAt < currentMillis &&
           state.noMovementThreshold &&
           ((currentMillis - state.lastFloorChangeAt) >
            (state.noMovementThreshold * NO_MOVEMENT_THRESHOLD_SCALE_MS)))
  {
    Serial.println(
        "**ALARM**: No movement between floors detected. Sending a Note.");
    send = true;
    alarm = true;
    state.alarmSent = true;
  }
  else if (currentMillis - lastUpdateMs >= updatePeriod)
  {
    send = true;
    Serial.println("Update period elapsed. Sending a Note.");
  }
  if (send)
  {
    displayReadings(readings);
    sendSensorReadings(readings, alarm);
    lastUpdateMs = currentMillis;
  }
  if (floorChange)
  {
    state.lastFloor = readings.currentFloor;
    state.lastFloorChangeAt = currentMillis;
  }
  return send;
}

void fetchEnvironmentVariables(applicationState &vars)
{
  J *req = notecard.newRequest("env.get");

  J *names = JAddArrayToObject(req, "names");
  JAddItemToArray(names, JCreateString("live"));
  JAddItemToArray(names, JCreateString("baseline_floor"));
  JAddItemToArray(names, JCreateString("floor_height"));
  JAddItemToArray(names, JCreateString("no_movement_threshold"));

  J *rsp = notecard.requestAndResponse(req);
  if (rsp != NULL)
  {
    if (notecard.responseError(rsp))
    {
      notecard.deleteResponse(rsp);
      return;
    }

    // Get the note's body
    J *body = JGetObject(rsp, "body");
    if (body != NULL)
    {
      int newBaselineFloor = atoi(JGetString(body, "baseline_floor"));
      if (newBaselineFloor != vars.baselineFloor)
      {
        setBaselineFloor = true;
        vars.baselineFloor = newBaselineFloor;
        vars.variablesUpdated = true;
      }

      float floorHeight = atof(JGetString(body, "floor_height"));
      if (floorHeight != vars.floorHeight)
      {
        vars.floorHeight = floorHeight;
        vars.variablesUpdated = true;
      }

      int noMovementThreshold = atoi(JGetString(body, "no_movement_threshold"));
      if (noMovementThreshold != vars.noMovementThreshold)
      {
        vars.noMovementThreshold = noMovementThreshold;
        vars.variablesUpdated = true;
      }

      char *liveStr = JGetString(body, "live");
      bool wasLive = vars.live;
      vars.live = (strcmp(liveStr, "true") == 0 || strcmp(liveStr, "1") == 0);
      if (vars.live && vars.live != wasLive)
      {
        // when the device becomes live, calibrate to the floor level given
        setBaselineFloor = true;
      }
      else if (vars.live != wasLive)
      {
        vars.variablesUpdated = true;
      }
      vars.alarmSent = false;
      updatePeriod = state.live ? LIVE_UPDATE_PERIOD : IDLE_UPDATE_PERIOD;

      Serial.printf("\nBaseline Floor: %d\n", vars.baselineFloor);
      Serial.print("Floor Height: ");
      Serial.print(vars.floorHeight);
      Serial.printf("\nMovement Threshold: %d\n",
                            vars.noMovementThreshold);
      Serial.printf("Live: %s\n\n", vars.live ? "true" : "false");
    }
  }
  notecard.deleteResponse(rsp);
}

bool pollEnvVars()
{
  if (millis() < nextPollMs)
  {
    return false;
  }

  nextPollMs = millis() + (ENV_POLL_SECS * 1000);

  J *rsp = notecard.requestAndResponse(notecard.newRequest("env.modified"));

  if (rsp == NULL)
  {
    return false;
  }

  uint32_t modifiedTime = JGetInt(rsp, "time");
  notecard.deleteResponse(rsp);
  if (lastModifiedTime == modifiedTime)
  {
    return false;
  }

  lastModifiedTime = modifiedTime;

  return true;
}

sensorReadings captureSensorReadings()
{
  sensorReadings readings;

  readings.readingTimestamp = millis();
  readings.temp = read_bar().temp;
  readings.pressure = read_bar().press;

  if (setBaselineFloor)
  {
    state.baselineChanged = true;
    resetFloorFilter(state.baselineFloor);
    state.baselineFloorPressure = readings.pressure;
    state.lastFloor =
        state.baselineFloor; // forget about the previous floor since this isn't
                             // an actual change in floor/altitude
    state.lastFloorChangeAt = millis();
    state.alarmSent = false;
    setBaselineFloor = false;
    Serial.println("Setting baseline floor / pressure");
    Serial.print("baseline floor: ");
    Serial.println(state.baselineFloor);
    Serial.print("baseline pressure:");
    Serial.print(state.baselineFloorPressure);
    Serial.println(" hPa");
  }

  readings.altitude = readAltitude(state.baselineFloorPressure);
  readings.floor =
      ((readings.altitude / state.floorHeight)) + state.baselineFloor;

  readings.currentFloor = (int)(readings.floor + FLOOR_OFFSET);

  return readings;
}

void sendSensorReadings(const sensorReadings &readings, bool alarm)
{
  J *req = notecard.newRequest("note.add");
  if (req != NULL)
  {
    JAddStringToObject(req, "file", "data.qo");
    Serial.println("Communication enabled");
    JAddBoolToObject(req, "sync", true);
    Serial.println("Sync Ready!");
    J *body = JCreateObject();
    if (body != NULL)
    {
      JAddNumberToObject(body, "floor", readings.currentFloor);
      JAddNumberToObject(body, "prevFloor", state.lastFloor);
      JAddNumberToObject(body, "altitude", readings.altitude);
      JAddNumberToObject(body, "pressure", readings.pressure);
      JAddNumberToObject(body, "temp", readings.temp);
      JAddNumberToObject(body, "direction",
                         readings.currentFloor - state.lastFloor);
      JAddStringToObject(body, "app", "nf1");
      JAddItemToObject(req, "body", body);
      
    }
    notecard.sendRequest(req);

    Serial.println("Sending floor.qo");
  }

  if (alarm)
  {
    J *req = notecard.newRequest("note.add");
    if (req != NULL)
    {
      JAddBoolToObject(req, "sync", true);
      JAddStringToObject(req, "file", "alarm.qo");
      J *body = JCreateObject();
      if (body != NULL)
      {
        JAddBoolToObject(body, "alarm", true);
        JAddStringToObject(body, "app", "nf1");
        JAddItemToObject(req, "body", body);
        notecard.sendRequest(req);

        Serial.println("Sending alarm.qo");
      }
    }
  }
}

void displayReadings(const sensorReadings &readings)
{
  Serial.print("Temperature = ");
  Serial.print(readings.temp);
  Serial.println(" *C");

  Serial.print("Pressure = ");
  Serial.print(readings.pressure);
  Serial.println(" hPa");

  Serial.print("Approx. Altitude = ");
  Serial.print(readings.altitude);
  Serial.println(" m");

  Serial.print("Previous Floor = ");
  Serial.println(state.lastFloor);

  Serial.print("Floor = ");
  Serial.println(readings.currentFloor);

  Serial.print("Floor = ");
  Serial.println(readings.floor);

  Serial.println();

#ifdef USE_DISPLAY
  display.clearDisplay();
  display.setCursor(0, 0);
  display.print("Press. ");
  display.println(readings.pressure);
  display.print("Alt.   ");
  display.println(readings.altitude);
  display.print("Floor: ");
  display.print(readings.currentFloor);
  display.display();
#endif
}

void NoteUserAgentUpdate(J *ua) {
    JAddStringToObject(ua, "app", "nf1");
}

