# CAN bus Vehicle Monitor using LoRaWAN and NB-IoT

## Introduction

This project includes the code for a professional IoT project related to decoding and sending data from a CAN Bus ISO 11898-2 or ISO11898-3 modes using the RAK13006 interface that has the MCP2518FD CAN controller. The communication will be done using LoRaWAN and NB-IoT.

## Hardware

For this tutorial, the next materials will be necessary:

[WisBlock Core RAK4631](https://store.rakwireless.com/products/rak4631-lpwan-node)

[WisBlock Base RAK19007](https://store.rakwireless.com/products/rak19007-wisblock-base-board-2nd-gen)

[CAN Bus Interface Microchip MCP2518FD](https://store.rakwireless.com/products/can-bus-interface-microchip-mcp2518fd-rak13006?_pos=1&_sid=9c882deb0&_ss=r)

And for the NB-IoT communication, we will use the Blues Wireless Products:

[Blues NoteCard](https://shop.blues.io/collections/notecard/products/note-nbgl-500)

[Blues Notecarrier B](https://blues.io/products/notecarrier/notecarrier-b/)

## Hardware Configuration

The hardware configuration is so easy! The WisBlock elements can easily be connected to the board and fixed using the screws. Connect the RAK13006 CAN Bus Interface Microchip MCP2518FD I/O Slot, and don't forget to connect the LoRaWAN® antenna.

![components](https://drive.google.com/file/d/1_-QAPF-9coiXJf-0QmAA-eVBymbUOAAv/view?usp=sharing)

The final result will look as the following:

![finalResult](https://drive.google.com/file/d/1ZxXeD5qLNk9Qdd2K9W3qjFpvYXOpKveH/view?usp=sharing)

The Blues components are easy to connect as well, just connect the Notecard on the Notecarrier and fix it with the screw. Don't forget to connect the antenna before using it.

![blues_board](https://drive.google.com/file/d/16v1RT5-pTJdML-JC3usvS3NPl9aGM01W/view?usp=sharing)


## Contents 

The Include folder has all the code for each module, the main, and a business logic code to accomplish the requirements. It is just a template, the user can change the logic completely. The main file, includes the code to read data and send it to LoRaWAN as the primary network and NB-IoT as the second network.

## How to use

This code was developed using PlatformIO in VS code, so, download or clone the folder and open it using VS code, if you are using Arduino, you can paste the code of the main file in the .ino file and create the headers, and paste the content of the files into the include folder.

[RAKwireless official Documentation Site](https://docs.rakwireless.com/Introduction/)

[BLues Developers Site](https://dev.blues.io/)
