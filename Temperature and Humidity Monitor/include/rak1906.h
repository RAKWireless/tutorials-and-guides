#ifndef RAK1906_H
#define RAK1906_H

#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME680.h> // Click to install library: http://librarymanager/All#Adafruit_BME680

Adafruit_BME680 bme;
struct bar_data{
  float humidity;
  float temp;
};
// Might need adjustments
#define SEALEVELPRESSURE_HPA (1010.0)

void bme680_init()
{
  Wire.begin();

  if (!bme.begin(0x76)) {
    Serial.println("Could not find a valid BME680 sensor, check wiring!");
    return;
  }

  // Set up oversampling and filter initialization
  bme.setTemperatureOversampling(BME680_OS_8X);
  bme.setHumidityOversampling(BME680_OS_2X);
  bme.setPressureOversampling(BME680_OS_4X);
  bme.setIIRFilterSize(BME680_FILTER_SIZE_3);
  bme.setGasHeater(320, 150); // 320*C for 150 ms
}



bar_data read_bar() {
    if (! bme.performReading())
  {
    Serial.println("Failed to perform reading :(");
  }
  bar_data sensor_reading;
  sensor_reading.humidity = bme.humidity;
  sensor_reading.temp = bme.temperature;
  return(sensor_reading);
}


#endif