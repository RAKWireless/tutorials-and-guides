// Union Structure
union u_tag_temp {
    uint8_t arr[4];
    float val;
} temp, hum;

// Callback function to receive and read data
void recv_cb(rui_lora_p2p_recv_t data) {
  for(int k=0; k<data.BufferSize; k++){
    if(k<4){
      temp.arr[k]=data.Buffer[k];
    }else if(k>=4 & k<8){
      hum.arr[k-4]=data.Buffer[k];
    }
  }
// Print Humidity and Temperature values
  Serial.print("Humidity: ");
  Serial.println(hum.val);
  Serial.print("Temperature: ");
  Serial.println(temp.val);
  }

void setup() {
  // Start the Serial
  Serial.begin(115200);
    
  // Configure LoRa p2p 
    Serial.printf("Set Node device work mode %s\r\n", api.lorawan.nwm.set(0) ? "Success" : "Fail");
    Serial.printf("Set P2P mode frequency %s\r\n", api.lorawan.pfreq.set(915000000) ? "Success" : "Fail");
    Serial.printf("Set P2P mode spreading factor %s\r\n", api.lorawan.psf.set(12) ? "Success" : "Fail");
    Serial.printf("Set P2P mode bandwidth %s\r\n", api.lorawan.pbw.set(125) ? "Success" : "Fail");
    Serial.printf("Set P2P mode code rate %s\r\n", api.lorawan.pcr.set(0) ? "Success" : "Fail");
    Serial.printf("Set P2P mode preamble length %s\r\n", api.lorawan.ppl.set(8) ? "Success" : "Fail");
    Serial.printf("Set P2P mode tx power %s\r\n", api.lorawan.ptp.set(22) ? "Success" : "Fail");
    api.lorawan.registerPRecvCallback(recv_cb); // Define callback function 
}

void loop() {
// Checking for new received data  
 if(api.lorawan.precv(3000)){
  delay(500);
 }
 else{
  delay(1000);
 }         
}

  
