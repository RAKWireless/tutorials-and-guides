#include "rak1901.h"

/*************************************

   LoRaWAN band setting:
     RAK_REGION_EU433
     RAK_REGION_CN470
     RAK_REGION_RU864
     RAK_REGION_IN865
     RAK_REGION_EU868
     RAK_REGION_US915
     RAK_REGION_AU915
     RAK_REGION_KR920
     RAK_REGION_AS923

 *************************************/

#define OTAA_BAND     (RAK_REGION_AU915) 
#define OTAA_DEVEUI   {0x60, 0x81, 0xF9, 0x7F, 0x45, 0x4C, 0x84, 0xFD}
#define OTAA_APPEUI   {0x60, 0x81, 0xF9, 0x62, 0xB9, 0x08, 0x55, 0x1E}
#define OTAA_APPKEY   {0xEB, 0x30, 0x13, 0x04, 0x04, 0x3A, 0x48, 0x20, 0x1D, 0x8C, 0x9C, 0x02, 0x39, 0x30, 0xA8, 0x23} 

rak1901 rak1901;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  pinMode(LED_BLUE, OUTPUT);
  digitalWrite(LED_BLUE, HIGH);
  Wire.begin(); // Begin function for I2C
  rak1901.init();//Start rak1901
  Serial.println("=============Temperature and Humidity Reading =========");
  
  void lora_init();
}

void loop() {
  // Turn on the blue LED
  digitalWrite(LED_BLUE, HIGH);
  delay(500);
  digitalWrite(LED_BLUE, LOW);
  delay(500);
  // Read the temperature and humidity values
    if (rak1901.update()) {
      temp.val_fl = rak1901.temperature();
      hum.val_fl = rak1901.humidity();
        
    } else {
       Serial.println("Please plug in the sensor RAK1901 and Reboot");
    }
    Serial.printf("Temperature = %.2f%°C\r\n", temp.val_fl);
    Serial.printf("Humidity = %.2f%%\r\n", hum.val_fl);

    // Fill the payload
  for(int j=0; j<4; j++){
    payload[j] = temp.val_arr[j];
  }

  for(int k=4; k<8; k++){
    payload[k] = hum.val_arr[k-4];
  }

  //Send data every 5 seconds

    Serial.printf("P2P send %s\r\n", api.lorawan.psend(sizeof(payload),payload)? "Success" : "Fail");
    delay(5000);
    
  }

  void lora_init()
{
   // OTAA Device EUI MSB first
  uint8_t node_device_eui[8] = OTAA_DEVEUI;
  // OTAA Application EUI MSB first
  uint8_t node_app_eui[8] = OTAA_APPEUI;
  // OTAA Application Key MSB first
  uint8_t node_app_key[16] = OTAA_APPKEY;

  if (!api.lorawan.appeui.set(node_app_eui, 8)) {
    Serial.printf("LoRaWan OTAA - set application EUI is incorrect! \r\n");
    return;
  }
  if (!api.lorawan.appkey.set(node_app_key, 16)) {
    Serial.printf("LoRaWan OTAA - set application key is incorrect! \r\n");
    return;
  }
  if (!api.lorawan.deui.set(node_device_eui, 8)) {
    Serial.printf("LoRaWan OTAA - set device EUI is incorrect! \r\n");
    return;
  }

  if (!api.lorawan.band.set(OTAA_BAND)) {
    Serial.printf("LoRaWan OTAA - set band is incorrect! \r\n");
    return;
  }
  if (!api.lorawan.deviceClass.set(RAK_LORA_CLASS_A)) {
    Serial.printf("LoRaWan OTAA - set device class is incorrect! \r\n");
    return;
  }
  if (!api.lorawan.njm.set(RAK_LORA_OTAA))  // Set the network join mode to OTAA
  {
    Serial.
  printf("LoRaWan OTAA - set network join mode is incorrect! \r\n");
    return;
  }
  if (!api.lorawan.join())  // Join to Gateway
  {
    Serial.printf("LoRaWan OTAA - join fail! \r\n");
    return;
  }

  /** Wait for Join success */
  while (api.lorawan.njs.get() == 0) {
    Serial.print("Wait for LoRaWAN join...");
    api.lorawan.join();
    delay(10000);
  }

  if (!api.lorawan.adr.set(true)) {
    Serial.printf
  ("LoRaWan OTAA - set adaptive data rate is incorrect! \r\n");
    return;
  }
  if (!api.lorawan.rety.set(1)) {
    Serial.printf("LoRaWan OTAA - set retry times is incorrect! \r\n");
    return;
  }
  if (!api.lorawan.cfm.set(1)) {
    Serial.printf("LoRaWan OTAA - set confirm mode is incorrect! \r\n");
    return;
  }

  /** Check LoRaWan Status*/
  Serial.printf("Duty cycle is %s\r\n", api.lorawan.dcs.get()? "ON" : "OFF"); // Check Duty Cycle status
  Serial.printf("Packet is %s\r\n", api.lorawan.cfm.get()? "CONFIRMED" : "UNCONFIRMED");  // Check Confirm status
  uint8_t assigned_dev_addr[4] = { 0 };
  api.lorawan.daddr.get(assigned_dev_addr, 4);
  Serial.printf("Device Address is %02X%02X%02X%02X\r\n", assigned_dev_addr[0], assigned_dev_addr[1], assigned_dev_addr[2], assigned_dev_addr[3]);  // Check Device Address
  Serial.println("");
}
