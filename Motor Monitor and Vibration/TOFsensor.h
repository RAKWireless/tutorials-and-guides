#ifndef TOFSENSOR_H
#define TOFSENSOR_H
#include "TOFsensor.h"


// Basic demo for accelerometer readings from Adafruit LIS3DH

/* This library was made by harnettlab.
The range readings are in units of mm. */
//Get AmbientRate and SignalRate amplitudes
//by modifying the pololu library
//Added some smoothing to the distance result.
#include <vl53l0xTOFA.h> http:  //librarymanager/All#vl53l0xTOFA
#include <Wire.h>

VL53L0xTOFA sensor;
float smoothLength = 0;
float signalrate = 0;
float ambientrate = 0;
float displacement;
float lastvalue = 0;
float runningAverage(int M) {
#define LM_SIZE 10
  static int LM[LM_SIZE];  // LastMeasurements
  static byte index = 0;
  static float sum = 0;
  static byte count = 0;

  // keep sum updated to improve speed.
  sum -= LM[index];
  LM[index] = M;
  sum += LM[index];
  index++;
  index = index % LM_SIZE;
  if (count < LM_SIZE) count++;

  return sum / count;
}

void configSensor(void) {
  pinMode(WB_IO2, OUTPUT);
  digitalWrite(WB_IO2, HIGH);
  Serial.begin(115200);
  Wire.begin();

  sensor.setTimeout(500);
  if (!sensor.init()) {
    Serial.println("Failed to detect and initialize sensor!");
    while (1) {}
  }

  // Start continuous back-to-back mode (take readings as
  // fast as possible).  To use continuous timed mode
  // instead, provide a desired inter-measurement period in
  // ms (e.g. sensor.startContinuous(100)).
  sensor.startContinuous();
}


void readSensor() {
   //smoothLength=runningAverage(sensor.readRangeContinuousMillimeters());
  sensor.readTOFA();
  smoothLength = runningAverage(sensor.tofa.distancemm);
  signalrate = sensor.tofa.signalrate;
  ambientrate = sensor.tofa.ambientrate;
  //calibLength=smoothLength/10*0.909-29.5 ; //uturn length in cm from fitting to two measurements only
  //Serial.print(smoothLength,2);//show it in cm
 // Serial.print(smoothLength);  //Print this values if you want to know them
 // Serial.print('\t');
 // Serial.print(signalrate);
 // Serial.print('\t');
//  Serial.print(ambientrate);

  if (smoothLength != lastvalue) {
    displacement = abs( smoothLength - lastvalue); // Calculates the displacement of the object"
    Serial.print("Displacement: ");
    Serial.println(displacement);
    lastvalue = smoothLength;
  } 

  if (sensor.timeoutOccurred()) {
    Serial.print(" TIMEOUT");
    Serial.println();
  }
}

float returnValueX() {
  return (displacement);
}

#endif
