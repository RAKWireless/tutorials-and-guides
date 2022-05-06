/**
 * @file EnvironMeter.ino
 * @author chris.mendez@rakwireless.com
 * @modifications by harold.duarte@rakwireless.com
 * @brief This sketch demonstrate reading data from a RAK1906 Environment Sensor and send it to cellphone through BLE.
 * @version 0.1
 * @date 2022-01-17
 * 
 * @copyright Copyright (c) 2022
 **/


#include <bluefruit.h>      // This one is already installed by the BSP.
#include "bsec.h"           // Click to install library: http://librarymanager/All#bsec
#include <U8g2lib.h>        // Click to install library: http://librarymanager/All#u8g2


// Helper functions declarations
void checkIaqSensorStatus(void);
void errLeds(void);
float readAltitude(float readPres, float seaLevel);

// Forward declarations for functions
void ble_connect_callback(uint16_t conn_handle);
void ble_disconnect_callback(uint16_t conn_handle, uint8_t reason);

#define PIN_VBAT WB_A0

uint32_t vbat_pin = PIN_VBAT;
float vbat_mv;
uint8_t vbat_per;

#define VBAT_MV_PER_LSB (0.73242188F) // 3.0V ADC range and 12 - bit ADC resolution = 3000mV / 4096
#define VBAT_DIVIDER_COMP (1.73)      // Compensation factor for the VBAT divider, depend on the board

#define REAL_VBAT_MV_PER_LSB (VBAT_DIVIDER_COMP * VBAT_MV_PER_LSB)

/**
   @brief  BLE UART service
   @note   Used for BLE UART communication
*/
BLEUart g_BleUart;

/** Flag if BLE UART client is connected */
bool g_BleUartConnected = false;

U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0);

//Sensor data
String sensordata1,sensordata2;
float temp, pres, hum, alti, gasi, iaqq, co2;
char data[32] = {0};

// Might need adjustments
#define SEALEVELPRESSURE_HPA (1013.25)

// Create an object of the class Bsec
Bsec iaqSensor;

String output;

/**
   @brief Get RAW Battery Voltage
*/
float readVBAT(void)
{
  float raw;

  // Get the raw 12-bit, 0..3000mV ADC value
  raw = analogRead(vbat_pin);

  return raw * REAL_VBAT_MV_PER_LSB;
}

/**
   @brief Convert from raw mv to percentage
   @param mvolts
      RAW Battery Voltage
*/
uint8_t mvToPercent(float mvolts)
{
  if (mvolts < 3300)
    return 0;

  if (mvolts < 3600)
  {
    mvolts -= 3300;
    return mvolts / 30;
  }

  mvolts -= 3600;
  return 10 + (mvolts * 0.15F); // thats mvolts /6.66666666
}

void bt_init()
{
  // Config the peripheral connection with maximum bandwidth
  // more SRAM required by SoftDevice
  // Note: All config***() function must be called before begin()
  Bluefruit.configPrphBandwidth(BANDWIDTH_MAX);
  Bluefruit.configPrphConn(92, BLE_GAP_EVENT_LENGTH_MIN, 16, 16);

  Bluefruit.begin(1, 0);
  // Set max power. Accepted values are: -40, -30, -20, -16, -12, -8, -4, 0, 4
  Bluefruit.setTxPower(4);
  // Set the BLE device name
  Bluefruit.setName("RAK4631_Environmeter");

  Bluefruit.Periph.setConnectCallback(ble_connect_callback);
  Bluefruit.Periph.setDisconnectCallback(ble_disconnect_callback);

  // Configure and Start BLE Uart Service
  g_BleUart.begin();

  // Set up and start advertising
  // Advertising packet
  Bluefruit.Advertising.addFlags(BLE_GAP_ADV_FLAGS_LE_ONLY_GENERAL_DISC_MODE);
  Bluefruit.Advertising.addTxPower();
  Bluefruit.Advertising.addName();

  /* Start Advertising
    - Enable auto advertising if disconnected
    - Interval:  fast mode = 20 ms, slow mode = 152.5 ms
    - Timeout for fast mode is 30 seconds
    - Start(timeout) with timeout = 0 will advertise forever (until connected)

    For recommended advertising interval
    https://developer.apple.com/library/content/qa/qa1931/_index.html
  */
  Bluefruit.Advertising.restartOnDisconnect(true);
  Bluefruit.Advertising.setInterval(32, 244); // in unit of 0.625 ms
  Bluefruit.Advertising.setFastTimeout(30);   // number of seconds in fast mode
  Bluefruit.Advertising.start(0);             // 0 = Don't stop advertising after n seconds
}

// Configuration
void setup(void)
{
  // Initialize the built in LED
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW);

  Serial.begin(115200);
  Wire.begin();

  // Set the analog reference to 3.0V (default = 3.6V)
  analogReference(AR_INTERNAL_3_0);

  // Set the resolution to 12-bit (0..4095)
  analogReadResolution(12); // Can be 8, 10, 12 or 14

  // Let the ADC settle
  delay(1);

  // Get a single ADC sample and throw it away
  readVBAT();

  time_t serial_timeout = millis();
  // On nRF52840 the USB serial is not available immediately
  while (!Serial)
  {
    if ((millis() - serial_timeout) < 5000)
    {
      delay(100);
      digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
    }
    else
    {
      break;
    }
  }
  
  u8g2.begin(); // OLED display init
  bt_init();    // BlE init

  iaqSensor.begin(BME680_I2C_ADDR_PRIMARY, Wire);
  output = "\nBSEC library version " + String(iaqSensor.version.major) + "." + String(iaqSensor.version.minor) + "." + String(iaqSensor.version.major_bugfix) + "." + String(iaqSensor.version.minor_bugfix);
  Serial.println(output);
  checkIaqSensorStatus();

  bsec_virtual_sensor_t sensorList[10] = {
    BSEC_OUTPUT_RAW_TEMPERATURE,
    BSEC_OUTPUT_RAW_PRESSURE,
    BSEC_OUTPUT_RAW_HUMIDITY,
    BSEC_OUTPUT_RAW_GAS,
    BSEC_OUTPUT_IAQ,
    BSEC_OUTPUT_STATIC_IAQ,
    BSEC_OUTPUT_CO2_EQUIVALENT,
    BSEC_OUTPUT_BREATH_VOC_EQUIVALENT,
    BSEC_OUTPUT_SENSOR_HEAT_COMPENSATED_TEMPERATURE,
    BSEC_OUTPUT_SENSOR_HEAT_COMPENSATED_HUMIDITY,
  };

  iaqSensor.updateSubscription(sensorList, 10, BSEC_SAMPLE_RATE_LP);
  checkIaqSensorStatus();
}

/**
   @brief  Callback when client connects
   @param  conn_handle: Connection handle id
*/
void ble_connect_callback(uint16_t conn_handle)
{
  (void)conn_handle;
  g_BleUartConnected = true;

  Serial.println("BLE client connected");
}

/**
   @brief  Callback invoked when a connection is dropped
   @param  conn_handle: connection handle id
   @param  reason: disconnect reason
*/
void ble_disconnect_callback(uint16_t conn_handle, uint8_t reason)
{
  (void)conn_handle;
  (void)reason;
  g_BleUartConnected = false;

  Serial.println("BLE client disconnected");
}

// Function that is looped forever
void loop(void)
{
  unsigned long time_trigger = millis();
  if (iaqSensor.run()) { // If new data is available
    temp = iaqSensor.temperature;
    hum = iaqSensor.humidity;
    pres = iaqSensor.pressure;
    alti = readAltitude(pres, SEALEVELPRESSURE_HPA);
    gasi = iaqSensor.gasResistance / 1000;
    iaqq = iaqSensor.iaq;
    co2 = iaqSensor.co2Equivalent;

    sensordata1 = "1," +String(temp) + "," + String(hum, 1) + "," + String(alti, 1);
    sensordata2 = "2," +String(iaqq, 0)+ "," + String(co2, 0);
    //sensordata = String(temp);
    // Get a raw ADC reading
    vbat_mv = readVBAT();

    // Convert from raw mv to percentage (based on LIPO chemistry)
    vbat_per = mvToPercent(vbat_mv);

    displaying();

    if (g_BleUartConnected)
    {
      g_BleUart.print(sensordata1);
      delay(100);
      g_BleUart.print(sensordata2);
    }

  } else {
    checkIaqSensorStatus();
  }
}

// Helper function definitions
void checkIaqSensorStatus(void)
{
  if (iaqSensor.status != BSEC_OK) {
    if (iaqSensor.status < BSEC_OK) {
      output = "BSEC error code : " + String(iaqSensor.status);
      Serial.println(output);
      for (;;)
        errLeds(); /* Halt in case of failure */
    } else {
      output = "BSEC warning code : " + String(iaqSensor.status);
      Serial.println(output);
    }
  }

  if (iaqSensor.bme680Status != BME680_OK) {
    if (iaqSensor.bme680Status < BME680_OK) {
      output = "BME680 error code : " + String(iaqSensor.bme680Status);
      Serial.println(output);
      for (;;)
        errLeds(); /* Halt in case of failure */
    } else {
      output = "BME680 warning code : " + String(iaqSensor.bme680Status);
      Serial.println(output);
    }
  }
}

void errLeds(void)
{
  delay(100);
  digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
}

void displaying() {
  u8g2.clearBuffer();         // clear the internal memory

//  // Battery percentage showing
//  u8g2.setFont(u8g2_font_5x8_tf); // choose a suitable font u8g2_font_ncenB10_tr
//
//  memset(data, 0, sizeof(data));
//  sprintf(data, "%d%%", vbat_per);
//  u8g2.drawStr(110, 10, data);

  // Environment data showing
  u8g2.setFont(u8g2_font_7x13_tf); // choose a suitable font u8g2_font_ncenB10_tr -- u8g2_font_profont11_tf

  memset(data, 0, sizeof(data));
  sprintf(data, "T:%.2f C", temp);
  u8g2.drawStr(3, 12, data);

  memset(data, 0, sizeof(data));
  snprintf(data, 64, "RH:%.2f %%", hum);
  u8g2.drawStr(3, 25, data);

  memset(data, 0, sizeof(data));
  sprintf(data, "A:%.2f m", alti);
  u8g2.drawStr(3, 37, data);

  memset(data, 0, sizeof(data));
  sprintf(data, "IAQ:%.2f ", iaqq);
  u8g2.drawStr(3, 49, data);

  memset(data, 0, sizeof(data));
  sprintf(data, "CO2:%.2f ppm", co2);
  u8g2.drawStr(3, 62, data);

  u8g2.sendBuffer(); // transfer internal memory to the display

}

float readAltitude(float readPres, float seaLevel) {
  float atmospheric = readPres / 100.0F;
  return 44330.0 * (1.0 - pow(atmospheric / seaLevel, 0.1903));
}
