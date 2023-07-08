# Indoor Floor-Level Tracker

## Introduction

This repository is a simple example about how to read data from a sensor and send it to the Cloud, using LoRaWAN and cellular networks.


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

### LoRaWAN communication

The process to communicate end devices via LoRaWAN networks can be done via a public network like [Helium](https://www.helium.com/) or set up your own network using a gateway for LoRaWAN. In this case, we will configure the second option. For this process you will need:

- [A gateways for LoRaWAN](https://store.rakwireless.com/collections/wisgate-edge)
- [TTN account](https://console.cloud.thethings.network/)

The gateway needs to be configured to work in base station mode, then it will be added to TTN creating a security token, finally creating the end devices with the credentials to be replaced in code as explained. before. This process is simple and can be done in a few minutes, check our [official documentation](https://docs.rakwireless.com/Product-Categories/WisGate/RAK7268-V2/Supported-LoRa-Network-Servers/#wisgateos-2-basics-station-to-ttnv3) for the step by step process.

### Blues Communication

This process is even simpler than setting up the LoRaWAN network, just create an account on the [Notehub page](https://notehub.io/sign-in?flow=473ef4b7-c19d-4440-a5d4-c2d1bb9d25b6) and create a new project. This project will be created with a unique identifier that needs to be added to the code, as mentioned above. You will need to add a device and add your Notecard that you are working with. You can review the Blues documentation to see the [process step by step](https://docs.rakwireless.com/Product-Categories/WisGate/RAK7268-V2/Supported-LoRa-Network-Servers/#wisgateos-2-basics-station-to-ttnv3).

DataCake Connection

Finally, the connection to DataCake will be made by integrating the two platforms, TTN and Notehub, with Datacake. In this part, you need to keep in mind that you are sending data with different payloads, so Datacake needs to be able to recognize this format and translate it to display the data in dashboards.

You can create two devices in Datacake or try to configure one device to receive data from different platforms. In either case, you must translate the payload so that the IoT platform can read it. To use this you will need to have a decoder function, the decoder functions examples are the `.js` files in the repo, just copy and paste on the DataCake platform.

Considering the above decoders, the process to connect Notehub and TTN with Datacake is not complicated. You can consult the complete [Blues](https://docs.datacake.de/integrations/blues-wireless-notecard) and [TTN](https://datacake.co/the-things-network-integration-datacake-iot-platform-dashboard-ttn-white-label) tutorial on Datacake official website.
