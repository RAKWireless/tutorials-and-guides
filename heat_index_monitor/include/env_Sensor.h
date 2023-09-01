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
//@brief function to calculate the heat index and return the value in Celsius
float heatIndexCalc(){
  float Ftemp = (bme680_get().temperature*(9/5))+32;
  float hum = bme680_get().humidity;
  float heatIndex = -42.379 + 2.04901523*Ftemp + 10.14333127*hum - .22475541*Ftemp*hum- .00683783*Ftemp*Ftemp- .05481717*hum*hum + .00122874*Ftemp*Ftemp*hum + .00085282*Ftemp*hum*hum - .00000199*Ftemp*Ftemp*hum*hum;
  

  if (hum < 13 && Ftemp > 80 && Ftemp < 112) {
    heatIndex -=  ((13-hum)/4)*sqrt((17-abs(Ftemp-95))/17);
  }

  else if (hum > 85 && Ftemp > 80 && Ftemp < 87) {
    heatIndex += ((hum-85)/10) * ((87-Ftemp)/5);
  }

  else if (Ftemp < 80) {
    heatIndex = 0.5 * (Ftemp + 61.0 + ((Ftemp-68.0)*1.2) + (hum*0.094));
  }
  float heatIndexC = (heatIndex-32.0)*5/9;
  return heatIndexC;
}

#endif

