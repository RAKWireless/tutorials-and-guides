#ifndef BAR_MODULE_H
#define BAR_MODULE_H

#include <Wire.h>
#include <Adafruit_LPS2X.h>
#include <Adafruit_Sensor.h>  // Install library

Adafruit_LPS22 g_lps22hb;
//Simple function to configure barometer
void conf_bar() {

  Serial.println("Adafruit LPS22 test!");

  // Try to initialize!
  if (!g_lps22hb.begin_I2C(0x5c)) 
  {
    Serial.println("Failed to find LPS22 chip");
    while (1) 
    { 
      delay(10); 
    }
  }

  Serial.println("LPS22 Found!");

  g_lps22hb.setDataRate(LPS22_RATE_10_HZ);
  Serial.print("Data rate set to: ");

  switch (g_lps22hb.getDataRate()) 
  {
    case LPS22_RATE_ONE_SHOT: Serial.println("One Shot / Power Down"); 
      break;
    case LPS22_RATE_1_HZ: Serial.println("1 Hz"); 
      break;
    case LPS22_RATE_10_HZ: Serial.println("10 Hz"); 
      break;
    case LPS22_RATE_25_HZ: Serial.println("25 Hz"); 
      break;
    case LPS22_RATE_50_HZ: Serial.println("50 Hz"); 
      break;

  }
}
// Read barometer values and return pressure value
float read_bar() {
  sensors_event_t pressure;
  sensors_event_t temp;
  g_lps22hb.getEvent(&pressure, &temp);
  return(pressure.pressure);
}

#endif