#include "rak1901.h"

rak1901 rak1901;
//union structure for read and store sensor data
union{
  float val_fl;
  uint8_t val_arr[4];
}temp, hum;

uint8_t payload[8];// simple payload to LoRaP2P

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  pinMode(LED_BLUE, OUTPUT);
  digitalWrite(LED_BLUE, HIGH);
  Wire.begin(); // Begin function for I2C
  rak1901.init();//Start rak1901
  Serial.println("=============Temperature and Humidity Reading =========");
  
// LoRa P2P Configuration
    Serial.printf("Set Node device work mode %s\r\n", api.lorawan.nwm.set(0) ? "Success" : "Fail");
    Serial.printf("Set P2P mode frequency %s\r\n", api.lorawan.pfreq.set(915000000) ? "Success" : "Fail");
    Serial.printf("Set P2P mode spreading factor %s\r\n", api.lorawan.psf.set(12) ? "Success" : "Fail");
    Serial.printf("Set P2P mode bandwidth %s\r\n", api.lorawan.pbw.set(125) ? "Success" : "Fail");
    Serial.printf("Set P2P mode code rate %s\r\n", api.lorawan.pcr.set(0) ? "Success" : "Fail");
    Serial.printf("Set P2P mode preamble length %s\r\n", api.lorawan.ppl.set(8) ? "Success" : "Fail");
    Serial.printf("Set P2P mode tx power %s\r\n", api.lorawan.ptp.set(22) ? "Success" : "Fail");
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
