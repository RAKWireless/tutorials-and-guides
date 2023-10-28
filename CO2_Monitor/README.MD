# LoRaWAN/Cellular CO2 Monitor

## Introduction

This repository includes a project to create an ready to use product to sense the CO2 concentration inside an environment. The idea is to make this project in order to make it work with WisBlock from RAKwireless and connecting data to the cloud using LoRaWAN and celullar networks.

With this solution it is possible to have a device capable of connecting to any other device using the modbus protocol and reading or writing the registers necessary for its control or monitoring.

## Necessary Hardware 
For this project the following RAKwireless products will be necessary:

- [WisBlock Baseboard RAK19001](https://store.rakwireless.com/products/rak19001-wisblock-dual-io-base-board)
- [WisBlock Core RAK4631](https://store.rakwireless.com/products/rak4631-lpwan-node?variant=37505443987654)
- [CO2 Sensor Sensirion SCD30](https://store.rakwireless.com/products/co2-sensor-sensirion-scd30-rak12037)
- [RAK Notecard Module](To be updated)

And the necessary Blues elements will be:

- [Blues Notecard](https://blues.io/products/notecard/) (Select one according your location)
- [Blues Notecarrier A, B or F](https://blues.io/products/notecarrier/) (Optional)



## Hardware Connection
Connecting the WisBlock elements is easy: just plug the core into the core slot, the CO2 sensor module into one IO slot and the WisBlock Notecard module in the other. Don't forget to connect the antennas in the devices.

![Materials](https://i.imgur.com/rmmsx1Om.jpg)

This time we will use the RAK19001 motherboard, but, [here](https://store.rakwireless.com/collections/wisblock-base) you can select another board. If you have any doubt about the Hardware configuration, you can visit the WisBlock official documentation [here](https://docs.rakwireless.com/Product-Categories/WisBlock/Quickstart/#hardware-setup). The final result that you will get is shown in the next figure:

![connection](https://i.imgur.com/xiufOfD.jpg)

After that you can put this project in an enclossure and you will get a final product ready to install, like the shown in the next Figure:

![Finalproduct](https://i.imgur.com/z9QqRNX.jpg)

## Firmware development 

## Some details

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

### LoRaWAN communication

The process to communicate end devices via LoRaWAN networks can be done via a public network like [Helium](https://www.helium.com/) or set up your own network using a gateway for LoRaWAN. In this case, we will configure the second option. For this process you will need:

- [A gateways for LoRaWAN](https://store.rakwireless.com/collections/wisgate-edge)
- [TTN account](https://console.cloud.thethings.network/)

The gateway needs to be configured to work in base station mode, then it will be added to TTN creating a security token, finally creating the end devices with the credentials to be replaced in code as explained. before. This process is simple and can be done in a few minutes, check our [official documentation](https://docs.rakwireless.com/Product-Categories/WisGate/RAK7268-V2/Supported-LoRa-Network-Servers/#wisgateos-2-basics-station-to-ttnv3) for the step by step process.

### Blues Communication

This process is even simpler than setting up the LoRaWAN network, just create an account on the [Notehub page](https://notehub.io/sign-in?flow=473ef4b7-c19d-4440-a5d4-c2d1bb9d25b6) and create a new project. This project will be created with a unique identifier that needs to be added to the code, as mentioned above. You will need to add a device and add your Notecard that you are working with. You can review the Blues documentation to see the [process step by step](https://docs.rakwireless.com/Product-Categories/WisGate/RAK7268-V2/Supported-LoRa-Network-Servers/#wisgateos-2-basics-station-to-ttnv3).

### AWS Connection

Connecting Notehub with AWS is simple, the data will be processed using AWS IoT Analytics, creating a channel, pipeline, data store and a database. With these tools created in AWS IoT Analytics, we will finally create a dashboard using Amazon Quicksight to display data. On the Notehub side you need to create a route. The complete process is described in the [Blues documentation](https://dev.blues.io/guides-and-tutorials/routing-data-to-cloud/aws-iot-analytics/). In the Notehub platform, you will need to create a JSONata Expression to convert data, if you don't know how to make it, the file Blues_Decoder.js containts the JSONata Expresion ready to use!.

The process to connect TTN to AWS is a bit complicated, so it requires a few steps: We'll connect TTN to AWS using AWS IoT Core, then connect the data to AWS IoT Analytics or any other AWS tool you want. The complete process to create this integration is shown on the [official TTN website](https://www.thethingsindustries.com/docs/integrations/cloud-integrations/aws-iot/). To create a Payload format you will need to edit the End device and in Payload format option, create a Uplink decoder. The code is available on the TTNDecoder.js file.

If you need more information you can visit the next sites:


[RAKwireless official Documentation Site](https://docs.rakwireless.com/Introduction/)

[Blues Wireless site](https://dev.blues.io/)