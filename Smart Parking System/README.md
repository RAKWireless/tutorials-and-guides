# Smart Parking Lot Help Request using LoRaWAN and NB-IoT

## Introduction

This project includes the code for a professional IoT project related Smart Parking Lot Help Request With Blueswireless And RAKwireless. The communication will be done using LoRaWAN and NB-IoT.

## Hardware

For this tutorial, the next materials will be necessary:

[WisBlock Core RAK4631](https://store.rakwireless.com/products/rak4631-lpwan-node)

[WisBlock Base RAK19007](https://store.rakwireless.com/products/rak19007-wisblock-base-board-2nd-gen)

[IO Extension Cable](https://store.rakwireless.com/products/wisblock-io-extension-cable-rak19008)

[E-Paper Module Solomon Systech Limited SSD1680](https://store.rakwireless.com/products/wisblock-epd-module-rak14000)

[Solar Panel](https://store.rakwireless.com/products/solar-panel)

[Unify Enclosure IP67 150x100x45mm](https://store.rakwireless.com/products/unify-enclosure-ip67-150-100-45mm?variant=42030277525702)

[WisBlock Blues Carrier Module](TBD)

And for the NB-IoT communication, we will use the Blues Wireless Products:

[Blues NoteCard](https://shop.blues.io/collections/notecard/products/note-nbgl-500)

[Blues Notecarrier B (Optional)](https://blues.io/products/notecarrier/notecarrier-b/)

## Hardware Configuration

The hardware configuration is so easy! The WisBlock elements can easily be connected to the board and fixed using the screws. Connect the RAK14000 E-Paper Module to the I/O Slot, and don't forget to connect the LoRaWAN® antenna.

![components](https://drive.google.com/file/d/1pHmqvrGAyAVoYfVE-5mqq0bjP5YBvjbH/view?usp=drive_link)

The final result will look as the following:

![finalResult](https://drive.google.com/file/d/1ByCxusJFdMnBVluHkQRzKyXhYzI-epa8/view?usp=drive_link)

The Blues components are easy to connect as well, just connect the Notecard on the Notecarrier and fix it with the screw. Don't forget to connect the antenna before using it.

![blues_board](https://drive.google.com/file/d/1ibqqZxUAxwkhT5K4Yz7a7NfJIKP3paDp/view?usp=drive_link)


## Contents 

The Include folder has all the code for each module, the main, and a business logic code to accomplish the requirements. It is just a template, the user can change the logic completely. The main file, includes the code to read data and send it to LoRaWAN as the primary network and NB-IoT as the second network.

## How to use

This code was developed using PlatformIO in VS code, so, download or clone the folder and open it using VS code, if you are using Arduino, you can paste the code of the main file in the .ino file and create the headers, and paste the content of the files into the include folder.

[RAKwireless official Documentation Site](https://docs.rakwireless.com/Introduction/)

[BLues Developers Site](https://dev.blues.io/)
