# RAKwireless Official Tutorials

## Introduction

This project include the code for a professional IoT project related with Tracking, it uses an acceleromenter to detect movement, a GPS module to measure the latitude and longitude, and a barometer, to determine altitude in indoor environments. The communication will be done using LoRaWAN and NB-IoT.

## Hardware

For this tutorial the next materials will be necessary:

[WisBlock Core RAK4631](https://store.rakwireless.com/products/rak4631-lpwan-node)

[WisBlock Base RAK19001](https://store.rakwireless.com/products/rak19001-wisblock-dual-io-base-board)

[3-Axis Acceleration Sensor RAK1904](https://store.rakwireless.com/products/rak1904-lis3dh-3-axis-acceleration-sensor)

[Barometric Pressure Sensor RAK1902](https://store.rakwireless.com/products/rak1902-kps22hb-barometric-pressure-sensor)

[GNSS GPS Location Module RAK12500](https://store.rakwireless.com/products/wisblock-gnss-location-module-rak12500)

And for the NB-IoT communication we will use the Blues Wireless Products:

[Blues NoteCard](https://shop.blues.io/collections/notecard/products/note-nbgl-500)

[Blues Notecarrier B](https://shop.blues.io/collections/notecarrier/products/carr-b)

## Hardware Configuration

The hardware configuration is so easy! The WisBlock elements can easily be connected to the board and be fixed using the screws. Conect the GNSS module in the Slot A, and the other sensors in the slots C y D to avoid incompatibility problems, and don't forget to connect the antennas.

![components](https://drive.google.com/file/d/1to8RPurrWjeNQQ2809vgRMZ5iM_yT4ir/view?usp=sharing)

The final result, will look as the following:

![finalResult](https://drive.google.com/file/d/1NiLrl8WdgEn_pG-B-XR4X1kLL0kojyYk/view?usp=sharing)

The Blues components are easy to connect as well, just connect the Notecard on the Notecarrier and fix it with the screw. Don't forget to connect the antenna before to use it.

![blues_board](https://drive.google.com/file/d/16v1RT5-pTJdML-JC3usvS3NPl9aGM01W/view?usp=sharing)


## Contents 

The Include folder, has all the code for each module, the main, and a business logic code to acomplish the requirements. It is just a template, the user can change the logic completely. The main file, include the code to read data and send it to LoRaWAN as primary network and NB-IoT as second network.

## How to use

This code was developed using PlatformIO in VS code, so, download or clone the folder and open it using VS code, if you are using Arduino, you can paste the code of the main file in the .ino file and create the headers, and paste the content of the files into the include folder.

[RAKwireless official Documentation Site](https://docs.rakwireless.com/Introduction/)

[BLues Developers Site](https://dev.blues.io/)
