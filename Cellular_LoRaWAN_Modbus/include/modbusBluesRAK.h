#ifndef MODBUSBLUESRAK_H
#define MODBUSBLUESRAK_H

#include <Adafruit_TinyUSB.h>
#include <Wire.h>
#include <ArduinoModbus.h>
#include <ArduinoRS485.h> //Click here to get the library: http://librarymanager/All#ArduinoRS485

void setupModbusClient() {

  Serial.println("Modbus RTU Client Toggle");

  // start the Modbus RTU client
  if (!ModbusRTUClient.begin(9600)) {
    Serial.println("Failed to start Modbus RTU Client!");
    while (1);
  }
  Serial.println("Device configured as Modbus client");
}
//@brief function to read a register
uint16_t readRegister() {
  uint16_t reg_value = 0;

  if (!ModbusRTUClient.requestFrom(0x00, HOLDING_REGISTERS, 0x07, 1))
  {
    Serial.print("failed to read registers! ");
    Serial.println(ModbusRTUClient.lastError());
  }
  else
  {
    reg_value = ModbusRTUClient.read();
  }
  return reg_value;
}

#endif