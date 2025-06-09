| <center><img src="./assets/rakstar.jpg" alt="RAKstar" width=25%></center>  | ![RAKWireless](./assets/RAK-Whirls.png) | [![Build Status](https://github.com/RAKWireless/RAK13006-MCP2518/workflows/RAK%20Library%20Build%20CI/badge.svg)](https://github.com/RAKWireless/RAK13006-MCP2518/actions) |
| -- | -- | -- |

# <RAK1306>

## 1. Introduction

RAK13006-MCP2518-Library is modified from Acan2517FD(V2.1.7) for pierremolinaro (https://github.com/pierremolinaro/acan2517FD). Because this library is very comprehensive and integrates many platfroms and supports MCP2517 and MCP2518. However we rearranged the directory structure and make two subclass to inherit original class, this way is very elegant. So this library completely inherits the functions of Acan2517FD and is more easy to use.

[*RAKWireless RAK13006 CAN*](https://store.RAKWireless.com/products/RAK13006-CAN)



## 2. Support

- Platfrom
  - most AVR boards
  - NRF52
  - ESP8266/ESP32
  - RP2040



- CAN Cotrollor
  - MCP2517 
  - MCP2518
  
    



## 3. Structure

The library structure is like below:

- datasheet
- assets
- example
- src
- keywords.txt
- library.json
- library.properties
- LICENSE.txt
- README.md



## 4. Function

**Notes: Because Class RAK_CAN and RAK_CAN_Settings Inherits from ACAN2517 and ACAN2517Settings class, all functions description below will apply to RAK_CAN and RAK_CAN_Settings  . Details usage of all API is in the doc catalogue** 



**RAK_CAN_Settings (const Oscillator inOscillator,const uint32_t inDesiredBitRate, const uint32_t inTolerancePPM = 1000);**   Constructor for RAK_CAN_Settings, Instantiates a new setting class using SPI.
Parameters:    

| Direction  | Name             | Function         |
| ---------- | ---------------- | :--------------- |
| Oscillator | inOscillator     | Oscillator type  |
| uint32_t   | inDesiredBitRate | BaudRate of CAN  |
| uint32_t   | inTolerancePPM   | Tolerance of PPM |
| return     | none             |                  |



**RAK_CAN::init(void)**

Initialization of class RAK_CAN, including the spi part in different platform.

Parameters:    None



**begin (const RAK_CAN_Settings & inSettings, void (* inInterruptServiceRoutine) (void)) ;**

Initialization of MCP2518 and add the IRQ handle.

Parameters:    

| Direction                 | Name                      | Function                 |
| ------------------------- | ------------------------- | :----------------------- |
| RAK_CAN_Settings          | RAK_CAN_Settings          | configuration of MCP2518 |
| inInterruptServiceRoutine | inInterruptServiceRoutine | IRQ handle               |
| uint32_t                  | errorCode                 | errorCode                |



**isr_core(void)**

IRQ handle, including the send, receive, FIFO...It must be called in the loop.

Parameters:    

| Direction | Name | Function |
| --------- | ---- | :------- |
| bool      | none | not used |



**tryToSend(const CANMessage &inMessage)**

Send message 

Parameters:    

| Direction  | Name       | Function       |
| ---------- | ---------- | :------------- |
| CANMessage | CANMessage | message of CAN |
| bool       | errorCode  | errorCode      |



**bool ACAN2517::receive(CANMessage &outMessage)**

Receive message 

Parameters:    

| Direction  | Name       | Function       |
| ---------- | ---------- | :------------- |
| CANMessage | CANMessage | message of CAN |
| bool       | errorCode  | errorCode      |



**available(void)**

Judgement before receive message 

Parameters:    

| Direction | Name | Function       |
| --------- | ---- | :------------- |
| bool      | none | receive or not |

