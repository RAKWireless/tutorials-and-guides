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

//@brief function to write coil values
void writeCoilValues(uint16_t id_device, uint16_t address, uint8_t value) {
  Serial.print("Writing Coil values ... ");

  ModbusRTUClient.beginTransmission(id_device, COILS, address, 1);
  
  ModbusRTUClient.write(value);
  
  if (!ModbusRTUClient.endTransmission()) {
    Serial.print("failed! ");
    Serial.println(ModbusRTUClient.lastError());
  } else {
    Serial.println("success");
  }

  // Alternatively, to write a single Coil value use:
  // ModbusRTUClient.coilWrite(...)
}
//@brief function to read coil values
uint8_t readCoilValues(uint16_t id_device, uint16_t address) {
  uint8_t data;
  Serial.print("Reading Coil values ... ");

  if (!ModbusRTUClient.requestFrom(id_device, COILS, address, 1)) {
    Serial.print("failed! ");
    Serial.println(ModbusRTUClient.lastError());
  } else {
    Serial.println("success");

    while (ModbusRTUClient.available()) {
      data = (uint8_t)(ModbusRTUClient.read());
      Serial.print(data);
      Serial.print(' ');
    }
    Serial.println();
  }
  return(data);
  // Alternatively, to read a single Coil value use:
  // ModbusRTUClient.coilRead(...)
}
// fucntion to read Discrete input Values
uint8_t readDiscreteInputValues(uint16_t id_device, uint16_t address) {
  uint8_t data;
  Serial.print("Reading Discrete Input values ... ");

  if (!ModbusRTUClient.requestFrom(id_device, DISCRETE_INPUTS, address)) {
    Serial.print("failed! ");
    Serial.println(ModbusRTUClient.lastError());
  } else {
    Serial.println("success");

    while (ModbusRTUClient.available()) {
      data = (uint16_t)(ModbusRTUClient.read());
      Serial.print(data);
      Serial.print(' ');
    }
    Serial.println();
  }
  return(data);
  // Alternatively, to read a single Discrete Input value use:
  // ModbusRTUClient.discreteInputRead(...)
}
// function to write a holding register
void writeHoldingRegisterValues(uint16_t id_device, uint16_t address, uint16_t value) {

  Serial.print("Writing Holding Registers values ... ");

  ModbusRTUClient.beginTransmission(id_device, HOLDING_REGISTERS, address, 1);
  for (int i = 0; i < 10; i++) {
    ModbusRTUClient.write(value);
  }
  if (!ModbusRTUClient.endTransmission()) {
    Serial.print("failed! ");
    Serial.println(ModbusRTUClient.lastError());
  } else {
    Serial.println("success");
  }

  // Alternatively, to write a single Holding Register value use:
  // ModbusRTUClient.holdingRegisterWrite(...)
}
// function to read Holding Registers
uint16_t readHoldingRegisterValues(uint16_t id_device, uint16_t address) {
  uint16_t data;
  Serial.print("Reading Input Register values ... ");

  if (!ModbusRTUClient.requestFrom(id_device, HOLDING_REGISTERS, address, 1)) {
    Serial.print("failed! ");
    Serial.println(ModbusRTUClient.lastError());
  } else {
    Serial.println("success");

    while (ModbusRTUClient.available()) {
      data = (uint16_t)(ModbusRTUClient.read());
      Serial.print(data);
      Serial.print(' ');
    }
    Serial.println();
  }
  return(data);
  // Alternatively, to read a single Holding Register value use:
  // ModbusRTUClient.holdingRegisterRead(...)
}
// Function to read Input Registers
uint16_t readInputRegisterValues(uint16_t id_device, uint16_t address) {
  uint16_t data;
  Serial.print("Reading input register values ... ");

  // read 10 discrete input values from (slave) id 42,
  if (!ModbusRTUClient.requestFrom(id_device, INPUT_REGISTERS, address, 1)) {
    Serial.print("failed! ");
    Serial.println(ModbusRTUClient.lastError());
  } else {
    Serial.println("success");

    while (ModbusRTUClient.available()) {
      data = (uint16_t)(ModbusRTUClient.read());
      Serial.print(data);
      Serial.print(' ');
    }
    Serial.println();
  }
  return(data);
  // Alternatively, to read a single Input Register value use:
  // ModbusRTUClient.inputRegisterRead(...)
}

#endif
