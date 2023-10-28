#ifndef   CO2_SENSOR
#define CO2_SENSOR

#include <Wire.h>
#include <Adafruit_TinyUSB.h> 
#include "SparkFun_SCD30_Arduino_Library.h" // Click here to get the library: http://librarymanager/All#SparkFun_SCD30

SCD30 airSensor;
// Data structure to read the values
struct data_sensor{
  float temp, hum, co2;
};
// Confiture the sensor
void setup_c02sensor()
{
  pinMode(WB_IO2, OUTPUT);
  digitalWrite(WB_IO2, HIGH);
  
	// Initialize Serial for debug output
	time_t timeout = millis();
	Serial.begin(115200);
	while (!Serial)
	{
		if ((millis() - timeout) < 5000)
		{
			delay(100);
		}
		else
		{
			break;
		}
	}

  Serial.println("SCD30 Basic Readings Example.");
	
  Wire.begin();

  if (airSensor.begin() == false)
  {
    Serial.println("Air sensor not detected. Please check wiring. Freezing...");
		while (1)
		{
			delay(10);
		}
  }
}
//@brief function to read data and return it as a structure
data_sensor read_data() {
  data_sensor sensorVars;
  if(airSensor.dataAvailable()){
    sensorVars.temp = airSensor.getTemperature();
    sensorVars.hum= airSensor.getHumidity();
    sensorVars.co2 = airSensor.getCO2();
  }

  else{
    Serial.println("Waiting for new data");
  }
  return sensorVars;
}

#endif
