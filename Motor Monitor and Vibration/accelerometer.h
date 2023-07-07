#ifndef ACCELEROMETER_H
#define ACCELEROMETER_H
#include "accelerometer.h"

#include <Arduino.h>
#include <Adafruit_LIS3DH.h>  //http://librarymanager/All#Adafruit_LIS3DH
#include <Adafruit_Sensor.h>
#include <Wire.h>


Adafruit_LIS3DH lis = Adafruit_LIS3DH();
//Max G force readable.  Can be: 2(default), 4, 8, 16
int Xvalue;
int Yvalue;
int Zvalue;

void configSensor() {

  Serial.begin(115200);
  while (!Serial) delay(10);  // will pause Zero, Leonardo, etc until serial console opens

  Serial.println("LIS3DH test!");

  if (!lis.begin(0x18)) {  // change this to 0x19 for alternative i2c address
    Serial.println("Couldnt start");
    while (1) yield();
  }
  Serial.println("LIS3DH found!");

  // lis.setRange(LIS3DH_RANGE_4_G);   // 2, 4, 8 or 16 G!

  Serial.print("Range = ");
  Serial.print(2 << lis.getRange());
  Serial.println("G");

  // lis.setDataRate(LIS3DH_DATARATE_50_HZ);
  Serial.print("Data rate set to: ");
  switch (lis.getDataRate()) {
    case LIS3DH_DATARATE_1_HZ: Serial.println("1 Hz"); break;
    case LIS3DH_DATARATE_10_HZ: Serial.println("10 Hz"); break;
    case LIS3DH_DATARATE_25_HZ: Serial.println("25 Hz"); break;
    case LIS3DH_DATARATE_50_HZ: Serial.println("50 Hz"); break;
    case LIS3DH_DATARATE_100_HZ: Serial.println("100 Hz"); break;
    case LIS3DH_DATARATE_200_HZ: Serial.println("200 Hz"); break;
    case LIS3DH_DATARATE_400_HZ: Serial.println("400 Hz"); break;

    case LIS3DH_DATARATE_POWERDOWN: Serial.println("Powered Down"); break;
    case LIS3DH_DATARATE_LOWPOWER_5KHZ: Serial.println("5 Khz Low Power"); break;
    case LIS3DH_DATARATE_LOWPOWER_1K6HZ: Serial.println("16 Khz Low Power"); break;
  }
}
// Function to get the average value


void readSensor() {
  // read the sensor value
  sensors_event_t event;
  lis.getEvent(&event);
  /* Display the results (acceleration is measured in m/s^2) */
  Xvalue = abs(event.acceleration.x*100);
  Yvalue = abs(event.acceleration.y*100);
  Zvalue = abs(event.acceleration.z*100);
  delay(200);
}

int returnValueX() {
  return (Xvalue);
}
int returnValueY() {
  return (Yvalue);
}
int returnValueZ() {
  return (Zvalue);
}

#endif
