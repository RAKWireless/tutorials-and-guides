#ifndef SOIL_HUM_SENSOR_H
#define SOIL_HUM_SENSOR_H

#include <Arduino.h>
#include <Adafruit_TinyUSB.h>
#include <Wire.h>
#include "RAK12035_SoilMoisture.h" // Click here to get the library: http://librarymanager/All#RAK12035_SoilMoisture

/** Sensor */
RAK12035 sensor;
//@brief structure to return sensor data
struct soilSensorData{
    uint16_t capacitance;
    uint8_t moisture;
    uint16_t temperature;
};

/** Set these two values after the sensor calibration was done */
uint16_t zero_val = 73;
uint16_t hundred_val = 250;

void setupSoilSensor()
{
	// Initialize the built in LED
	pinMode(LED_GREEN, OUTPUT);
	digitalWrite(LED_GREEN, LOW);

	// Initialize the connection status LED
	pinMode(LED_BLUE, OUTPUT);
	digitalWrite(LED_BLUE, HIGH);
	digitalWrite(LED_GREEN, LOW);

	// Initialize sensor
	sensor.begin();

	// Get sensor firmware version
	uint8_t data = 0;
	sensor.get_sensor_version(&data);
	Serial.print("Sensor Firmware version: ");
	Serial.println(data, HEX);
	Serial.println();

	// Set the calibration values
	// Reading the saved calibration values from the sensor.
	sensor.get_dry_cal(&zero_val);
	sensor.get_wet_cal(&hundred_val);
	Serial.printf("Dry calibration value is %d\n", zero_val);
	Serial.printf("Wet calibration value is %d\n", hundred_val);
}

soilSensorData readSoilSensor()
{
    soilSensorData SensorDataSoil;
	// Read capacitance
	sensor.get_sensor_capacitance(&SensorDataSoil.capacitance);

	// Read moisture in %
	// after calibration, we get the Capacitance in air and in water, like zero_val and B. zero_val means 0% and B means 100%.
	// So the humidity is humidity =  (Capacitance-A) / ((B-A)/100.0)
	sensor.get_sensor_moisture(&SensorDataSoil.moisture);

	// Read temperature
	sensor.get_sensor_temperature(&SensorDataSoil.temperature);

    return(SensorDataSoil);
}

#endif