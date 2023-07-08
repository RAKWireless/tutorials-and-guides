# Indoor Floor-Level Tracker

## Introduction

This repository includes a project replicated from the first Blues Wireless' accelerator. The idea is to modify this project in order to make it work with WisBlock from RAKwireless and connecting data to the cloud using LoRaWAN and celullar networks.

The project proposes a way to calibrate the sensor by taking a first reading taken at the base of a building and then verifying the change of the floors according to the atmospheric pressure readings read and compared with the reference.

## Necessary Hardware 
For this project the following RAKwireless products will be necessary:

- [WisBlock Baseboard RAK19007](https://store.rakwireless.com/products/rak19007-wisblock-base-board-2nd-gen?_pos=1&_sid=042c9c083&_ss=r)
- [WisBlock Core RAK4631](https://store.rakwireless.com/products/rak4631-lpwan-node?variant=37505443987654)
- [Barometric Pressure sensor RAK1902](https://store.rakwireless.com/products/rak1902-kps22hb-barometric-pressure-sensor)
- WisBlock Blues Carrier Module (To be updated)
- [Water Proof Barometric Pressure Sensor STMicroelectronics LPS33HW](https://store.rakwireless.com/products/wisblock-barometer-wt-sensor-rak12011) (Optional)


And the necessary Blues elements will be:

- [Blues Notecard](https://blues.io/products/notecard/) (Select one according your location)
- [Blues Notecarrier A, B or F](https://blues.io/products/notecarrier/) (Optional)


## Hardware Connection
Connecting the WisBlock elements is easy: just plug the core into the core slot and the barometer sensor in any sensor slot of the card. This time we will use the RAK19007 motherboard, but, [here](https://store.rakwireless.com/collections/wisblock-base) you can select another board. If you have any doubt about the Hardware configuration, you can visit the WisBlock official documentation [here](https://docs.rakwireless.com/Product-Categories/WisBlock/Quickstart/#hardware-setup). The final result that you will get is shown in the next figure:

![connection](https://i.imgur.com/ibAVC3qm.jpg)(To be updated)

After that you can put this project in an enclossure and you will get a final product ready to install, like the shown in the next Figure:

![finalproduct](https://i.imgur.com/Bu8KZKCm.jpg)

## Firmware development 

- [Here](https://github.com/RAKWireless/tutorials-and-guides/tree/master/Indoor_floor_level_tracker/floor_sensor_readings) you will find the firmware to read data from the sensor and send it to DataCake using LoRaWAN and Cellular network
- [Here](https://github.com/RAKWireless/tutorials-and-guides/tree/master/Indoor_floor_level_tracker/floor_sensor_readings) you will find the firmware and instructions to replicate the Blues accelerator using WisBlock.

If you need more information you can visit the next sites:


[RAKwireless official Documentation Site](https://docs.rakwireless.com/Introduction/)

[Blues Wireless site](https://dev.blues.io/)
