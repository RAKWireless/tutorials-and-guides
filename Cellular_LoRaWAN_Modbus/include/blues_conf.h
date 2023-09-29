#ifndef BLUES_CONF_H
#define BLUES_CONF_H

#include <Arduino.h>
#include <Wire.h>
#include <Notecard.h>
#include "modbusBluesRAK.h"

// Add the Product UID gotten in Blues Notehub
#define PRODUCT_UID " "
Notecard notecard;
//@brief function to config Blues Notecard
void blues_card_conf(){
    notecard.begin();
  
    J *req = notecard.newRequest("hub.set");
    if (PRODUCT_UID[0]) {
        JAddStringToObject(req, "product", PRODUCT_UID);
    }

    JAddStringToObject(req, "mode", "continuous");

    notecard.sendRequest(req);
    Serial.println("Notecard configured!");
}
//@brief function to read registers and send data using Blues 
void send_data_using_blues(){

    J *req = notecard.newRequest("note.add");
    if (req != NULL) {
    JAddStringToObject(req, "file", "data.qo");
    Serial.println("Communication enabled");
    JAddBoolToObject(req, "sync", true);
    Serial.println("Sync Ready!");

    J *body = JCreateObject();
    if (body != NULL) {
    // This functions is just read a coil, a holding register and an input register
    // All of them are stored on the device 0X00 with address 0x00
    // Change the code according your needs
      JAddNumberToObject(body, "coil", (double)(readCoilValues(0x00, 0x00)));
      JAddNumberToObject(body, "holdingRegister", (double)(readHoldingRegisterValues(0x00, 0x00)));
      JAddNumberToObject(body, "inputRegister", (double)(readInputRegisterValues(0x00, 0x00)));
      JAddItemToObject(req, "body", body);
    }
    notecard.sendRequest(req);
    Serial.println("It worked!");
  }
}



#endif
