# Tracking Assets using LoRaWAN and NB-IoT

## Introduction

This project include the code for a professional IoT project related with monitoring temperature and humidity, it uses a Environment Sensor BOSCH BME680, to determine temperature and humidity in a indoor environment. The communication will be done using LoRaWAN and NB-IoT.

## Hardware

For this tutorial the next materials will be necessary:

[WisBlock Core RAK4631](https://store.rakwireless.com/products/rak4631-lpwan-node)

[WisBlock Base RAK19001](https://store.rakwireless.com/products/rak19001-wisblock-dual-io-base-board)

[Environment Sensor BOSCH BME680 RAK1906](https://store.rakwireless.com/products/rak1906-bme680-environment-sensor)

And for the NB-IoT communication we will use the Blues Wireless Products:

[Blues NoteCard](https://shop.blues.io/collections/notecard/products/note-nbgl-500)

[Blues Notecarrier A](https://blues.io/products/notecarrier/notecarrier-a/)

## Hardware Configuration

The hardware configuration is so easy! The WisBlock elements can easily be connected to the board and be fixed using the screws. Conect the RAK1906 Environment Sensor BOSCH BME680 to the Slot A, and don't forget to connect the LoRaWAN® antenna.

![components](https://drive.google.com/file/d/1_-QAPF-9coiXJf-0QmAA-eVBymbUOAAv/view?usp=sharing)

The final result, will look as the following:

![finalResult](https://drive.google.com/file/d/1ZxXeD5qLNk9Qdd2K9W3qjFpvYXOpKveH/view?usp=sharing)

The Blues components are easy to connect as well, just connect the Notecard on the Notecarrier and fix it with the screw. Don't forget to connect the antenna before to use it.

![blues_board](https://drive.google.com/file/d/16v1RT5-pTJdML-JC3usvS3NPl9aGM01W/view?usp=sharing)


## Contents 

The Include folder, has all the code for each module, the main, and a business logic code to acomplish the requirements. It is just a template, the user can change the logic completely. The main file, include the code to read data and send it to LoRaWAN as primary network and NB-IoT as second network.

## How to use

This code was developed using PlatformIO in VS code, so, download or clone the folder and open it using VS code, if you are using Arduino, you can paste the code of the main file in the .ino file and create the headers, and paste the content of the files into the include folder.

[RAKwireless official Documentation Site](https://docs.rakwireless.com/Introduction/)

[BLues Developers Site](https://dev.blues.io/)
