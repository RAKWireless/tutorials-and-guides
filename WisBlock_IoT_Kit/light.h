#include "Light_VEML7700.h"  ////http://librarymanager/All#Light_veml7700
Light_VEML7700 VMEL = Light_VEML7700();
#include <Wire.h>
#include <U8g2lib.h>
#include "tempandhumd.h"
U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0);

double Lux;
double White;
double RawALS;
void configSensorLIGHT() {


  // Initialize Serial for debug output

  u8g2.begin();

  pinMode(WB_IO2, OUTPUT);
  digitalWrite(WB_IO2, HIGH);
  delay(300);
  time_t timeout = millis();
  while (!Serial) {
    if ((millis() - timeout) < 5000) {
      delay(100);
    } else {
      break;
    }
  }
  if (!VMEL.begin()) {
    Serial.println("Sensor not found");
    while (1)
      ;
  }

  VMEL.setGain(VEML7700_GAIN_1);
  VMEL.setIntegrationTime(VEML7700_IT_800MS);

  Serial.print(F("Gain: "));
  switch (VMEL.getGain()) {
    case VEML7700_GAIN_1: Serial.println("1"); break;
    case VEML7700_GAIN_2: Serial.println("2"); break;
    case VEML7700_GAIN_1_4: Serial.println("1/4"); break;
    case VEML7700_GAIN_1_8: Serial.println("1/8"); break;
  }

  Serial.print(F("Integration Time (ms): "));
  switch (VMEL.getIntegrationTime()) {
    case VEML7700_IT_25MS: Serial.println("25"); break;
    case VEML7700_IT_50MS: Serial.println("50"); break;
    case VEML7700_IT_100MS: Serial.println("100"); break;
    case VEML7700_IT_200MS: Serial.println("200"); break;
    case VEML7700_IT_400MS: Serial.println("400"); break;
    case VEML7700_IT_800MS: Serial.println("800"); break;
  }

  //veml.powerSaveEnable(true);
  //veml.setPowerSaveMode(VEML7700_POWERSAVE_MODE4);

  VMEL.setLowThreshold(10000);
  VMEL.setHighThreshold(20000);
  VMEL.interruptEnable(true);
}

void bme680_get() {
  char data[32] = { 0 };
  Lux = VMEL.readLux();
  White = VMEL.readWhite();
  RawALS = VMEL.readALS();
  uint16_t lightlux = returntemperature();

  // display bme680 sensor data on OLED
  u8g2.clearBuffer();                  // clear the internal memory
  u8g2.setFont(u8g2_font_ncenB10_tr);  // choose a suitable font

  memset(data, 0, sizeof(data));
  sprintf(data, "LUX %.2f", Lux);
  u8g2.drawStr(3, 15, data);

  memset(data, 0, sizeof(data));
  snprintf(data, 64, "RawALS %.2f", RawALS);
  u8g2.drawStr(3, 30, data);

  memset(data, 0, sizeof(data));
  sprintf(data, "White %.2f", White);
  u8g2.drawStr(3, 45, data);

  memset(data, 0, sizeof(data));
  sprintf(data, "Temperature %.2f", White);
  u8g2.drawStr(3, 45, data);

  u8g2.sendBuffer();  // transfer internal memory to the display
}


void readlight() {
  Serial.print("Lux: ");
  Serial.println(VMEL.readLux());
  Serial.print("White: ");
  Serial.println(VMEL.readWhite());
  Serial.print("Raw ALS: ");
  Serial.println(VMEL.readALS());
  uint16_t irq = VMEL.interruptStatus();
  if (irq & VEML7700_INTERRUPT_LOW) {
    Serial.println("Low threshold");
  }
  if (irq & VEML7700_INTERRUPT_HIGH) {
    Serial.println("High threshold");
  }
  delay(500);

  bme680_get();
}

int returnlux() {
  return (Lux);
}

int returnwhite() {
  return (White);
}

int returnRAWALS() {
  return (RawALS);
}
