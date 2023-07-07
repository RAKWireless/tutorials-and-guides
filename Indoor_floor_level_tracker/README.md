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

When working with LoRaWAN, you need to choose a LNS (either TTN, Chirpstack, etc), it will provide you credentials whether you are using OTAA or ABP connection mode, modify your code with your credentials to successfully connect to the network.

```
//OTAA keys !!!! KEYS ARE MSB !!!!
uint8_t nodeDeviceEUI[8] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
uint8_t nodeAppEUI[8] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
uint8_t nodeAppKey[16] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
// ABP keys
uint32_t nodeDevAddr = 0x260116F8;
uint8_t nodeNwsKey[16] = {0x7E, 0xAC, 0xE2, 0x55, 0xB8, 0xA5, 0xE2, 0x69, 0x91, 0x51, 0x96, 0x06, 0x47, 0x56, 0x9D, 0x23};
uint8_t nodeAppsKey[16] = {0xFB, 0xAC, 0xB6, 0x47, 0xF3, 0x58, 0x45, 0xC7, 0x50, 0x7D, 0xBF, 0x16, 0x8B, 0xA8, 0xC1, 0x7C};
```

The LoRaWAN code works with events, that means you don't need to add anything in the loop function for the code to work correctly, just modify the sending time and buffer size parameters that you need to transmit in the `lorawan_code.h`.

```
/ Private definition
#define LORAWAN_APP_DATA_BUFF_SIZE 64 /**< buffer size of the data to be transmitted. */
#define LORAWAN_APP_INTERVAL 60000 /**< Defines for user timer, the application data transmission interval. 20s, value in [ms]. */
```
Finally, if you are sending data using the Notecard, you need to first send the data to the Notehub, the blues platform to work as a connection between our end device and the IoT platform. For that, you need to edit the `blues_conf.h` file and define your `PRODUCT_UID` according to your credential in your Notehub account.

```
#define PRODUCT_UID "com.company.username:projectname"
```

## Sending Data to the Cloud

The data path shown in yellow is LoRaWAN communication, and the one shown in purple is the path that data follows using NB-IoT/LTE. Obviously, there are many more components involved in this process, but the components shown in the figure are the ones that users need to configure.

If the end device is connected and programmed, the next part is to configure the LoRaWAN path.

### LoRaWAN communication

The process to communicate end devices via LoRaWAN networks can be done via a public network like Helium or set up your own network using a gateway for LoRaWAN. In this case, we will configure the second option. For this process you will need:

- A gateways for LoRaWAN
- TTN account

The gateway needs to be configured to work in base station mode, then it will be added to TTN creating a security token, finally creating the end devices with the credentials to be replaced in code as explained. before. This process is simple and can be done in a few minutes, check our [official documentation](https://docs.rakwireless.com/Product-Categories/WisGate/RAK7268-V2/Supported-LoRa-Network-Servers/#wisgateos-2-basics-station-to-ttnv3) for the step by step process.

### Blues Communication

This process is even simpler than setting up the LoRaWAN network, just create an account on the [Notehub page](https://notehub.io/sign-in?flow=473ef4b7-c19d-4440-a5d4-c2d1bb9d25b6) and create a new project. This project will be created with a unique identifier that needs to be added to the code, as mentioned above. You will need to add a device and add your Notecard that you are working with. You can review the Blues documentation to see the [process step by step](https://docs.rakwireless.com/Product-Categories/WisGate/RAK7268-V2/Supported-LoRa-Network-Servers/#wisgateos-2-basics-station-to-ttnv3).

DataCake Connection

Finally, the connection to DataCake will be made by integrating the two platforms, TTN and Notehub, with Datacake. In this part, you need to keep in mind that you are sending data with different payloads, so Datacake needs to be able to recognize this format and translate it to display the data in dashboards.

You can create two devices in Datacake or try to configure one device to receive data from different platforms. In either case, you must translate the payload so that the IoT platform can read it. To use this you will need to have a decoder function. Here is an example for the LoRaWAN and here there is another for Blues.

Considering the above decoders, the process to connect [Notehub](https://docs.datacake.de/integrations/blues-wireless-notecard) and [TTN](https://datacake.co/the-things-network-integration-datacake-iot-platform-dashboard-ttn-white-label) with Datacake is not complicated. You can consult the complete Blues and TTN tutorial on Datacake official website.


[RAKwireless official Documentation Site](https://docs.rakwireless.com/Introduction/)

[RAKwireless Hackster Official Channel](https://www.hackster.io/rak-wireless)
