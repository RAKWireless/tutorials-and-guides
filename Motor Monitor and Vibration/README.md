# Motor Monitor and Vibration using LoRaWAN and NB-IoT

## Introduction

This project includes the code for a professional IoT project related to Motor monitoring and vibration, it uses 3-Axis Acceleration Sensor by STMicroelectronics LIS3DH to detect and measure the vibration of a motor. The communication will be done using LoRaWAN and NB-IoT.

## Hardware

For this tutorial, the following materials will be necessary:

[WisBlock Core RAK4631](https://store.rakwireless.com/products/rak4631-lpwan-node)

[WisBlock Base RAK19007](https://store.rakwireless.com/products/rak19007-wisblock-base-board-2nd-gen)

[3-Axis Acceleration Sensor](https://store.rakwireless.com/products/rak1904-lis3dh-3-axis-acceleration-sensor)

[Sensor Extension Cable](https://store.rakwireless.com/products/fpc-extension-cable-for-slot-a-to-d-rak19005)

And for the NB-IoT communication, we will use the Blues Wireless Products:

[Blues NoteCard](https://shop.blues.io/collections/notecard/products/note-nbgl-500)

[Blues Notecarrier B](https://shop.blues.io/collections/notecarrier/products/carr-b)

## Hardware Configuration

The hardware configuration is so easy! The WisBlock elements can easily be connected to the board using the screws. Connect the GNSS module in Slot A, and the other sensors in the slots C y D to avoid incompatibility problems, and don't forget to connect the antennas.

![components]() [Peding to upload]

The final result will look as the following:

![finalResult]() [Peding to upload]

The Blues components are easy to connect as well, just connect the Notecard on the Notecarrier and fix it with the screw. Please don't forget to connect the antenna before you use it.

![blues_board]() [Peding to upload]


## Contents 

The Include folder has all the code for each module, the main, and a business logic code to accomplish the requirements. It is just a template, the user can change the logic completely. The main file includes the code to read data and send it to LoRaWAN as the primary network and NB-IoT as the second network.

## How to use

This code was developed using PlatformIO in VS code, so, download or clone the folder and open it using VS code, if you are using Arduino, you can paste the code of the main file in the .ino file and create the headers, and paste the content of the files into the include folder.

[RAKwireless official Documentation Site](https://docs.rakwireless.com/Introduction/)

[BLues Developers Site](https://dev.blues.io/)
