# Indoor Particulate Matter Monitor using LoRaWAN and NB-IoT

## Introduction

This project includes the code for a professional IoT project Indoor Particulate Matter Monitor from PM0.3 to PM100, it uses an Particle Matter Sensor Plantower PMSA003I, to determine the concentration of particles in the air with sizes of 1.0, 2.5 and 10μm. The communication will be done using LoRaWAN and NB-IoT.

## Hardware

For this tutorial, the next materials will be necessary:

[WisBlock Core RAK4631](https://store.rakwireless.com/products/rak4631-lpwan-node)

[WisBlock Base RAK19001](https://store.rakwireless.com/products/rak19001-wisblock-dual-io-base-board)

[Particle Matter Sensor Plantower PMSA003I](https://store.rakwireless.com/products/particle-matter-sensor-plantower-pmsa003i-rak12039)

[Unify Enclosure IP67 150x100x45mm](https://store.rakwireless.com/products/unify-enclosure-ip67-150-100-45mm?variant=42030277525702)

And for the NB-IoT communication, we will use the Blues Wireless Products:

[Blues NoteCard](https://shop.blues.io/collections/notecard/products/note-nbgl-500)

[Blues Notecarrier A](https://blues.io/products/notecarrier/notecarrier-a/)

## Hardware Configuration

The hardware configuration is so easy! The WisBlock elements can easily be connected to the board and fixed using the screws. Connect the RAK12039 Particle Matter Sensor Plantower PMSA003I to Slot B, and don't forget to connect the LoRaWAN® antenna.

![components](https://drive.google.com/file/d/11YlHk2q7Y84cyRPtpLYtq8H-N0pWuqm2/view?usp=sharing)

The final result will look as the following:

![finalResult](https://drive.google.com/file/d/1BJFPhou9cw6Y4sgCDfUWD2HKtZB-LlIr/view?usp=sharing)

The Blues components are easy to connect as well, just connect the Notecard on the Notecarrier and fix it with the screw. Don't forget to connect the antenna before using it.



## Contents 

The Include folder has all the code for each module, the main, and a business logic code to accomplish the requirements. It is just a template, the user can change the logic completely. The main file, includes the code to read data and send it to LoRaWAN as the primary network and NB-IoT as the second network.

## How to use

This code was developed using PlatformIO in VS code, so, download or clone the folder and open it using VS code, if you are using Arduino, you can paste the code of the main file in the .ino file and create the headers, and paste the content of the files into the include folder.

[RAKwireless official Documentation Site](https://docs.rakwireless.com/Introduction/)

[Blues Developers Site](https://dev.blues.io/)
