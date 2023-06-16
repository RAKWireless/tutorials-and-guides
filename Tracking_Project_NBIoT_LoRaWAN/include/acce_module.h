#ifndef ACCE_MODULE_H
#define ACCE_MODULE_H

#include <Arduino.h>
#include "SparkFunLIS3DH.h" //http://librarymanager/All#SparkFun-LIS3DH
#include <Wire.h>

LIS3DH SensorTwo(I2C_MODE, 0x18);
//Simple structure to store data
struct acc_data{
	float x, y, z;
};
//function to read the sensor data
acc_data read_acc(){
	acc_data acc_buffer;
	acc_buffer.x = SensorTwo.readFloatAccelX();
	acc_buffer.y = SensorTwo.readFloatAccelY();
	acc_buffer.z = SensorTwo.readFloatAccelZ();

	return(acc_buffer);
}
//Configure accelerometer
void config_acc()
{

	if (SensorTwo.begin() != 0)
	{
		Serial.println("Problem starting the sensor at 0x18.");
	}
	else
	{
		Serial.println("Sensor at 0x18 started.");
		// Set low power mode
		uint8_t data_to_write = 0;
		SensorTwo.readRegister(&data_to_write, LIS3DH_CTRL_REG1);
		data_to_write |= 0x08;
		SensorTwo.writeRegister(LIS3DH_CTRL_REG1, data_to_write);
		delay(100);

		data_to_write = 0;
		SensorTwo.readRegister(&data_to_write, 0x1E);
		data_to_write |= 0x90;
		SensorTwo.writeRegister(0x1E, data_to_write);
		delay(100);
	}
	Serial.println("enter !");
}

#endif