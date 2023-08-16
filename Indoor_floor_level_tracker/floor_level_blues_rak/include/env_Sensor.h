#ifndef ENV_SENSOR_H
#define ENV_SENSOR_H

#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME680.h> // Click to install library: http://librarymanager/All#Adafruit_BME680

Adafruit_BME680 bme;
// Might need adjustments
#define SEALEVELPRESSURE_HPA (1013.0)
//@brief structure to return sensor data
struct bme680Data {
  float temperature, pressure, humidity, gas_resistance;
};
//@brief function to initialize sensor
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
//@brief function to read and return data sensor
bme680Data bme680_get()
{
  bme680Data env_data;
    if (! bme.performReading())
  {
    Serial.println("Failed to perform reading :(");
  }

  env_data.temperature = bme.temperature;
  env_data.humidity = bme.humidity;
  env_data.pressure = bme.pressure/100.0;
  env_data.gas_resistance = bme.gas_resistance/1000.0;

  return(env_data);
}

float readAltitude(float seaLevel) {
  float atmospheric = bme680_get().pressure;
  return 44330.0 * (1.0 - pow(atmospheric / seaLevel, 0.1903));
}

#endif

