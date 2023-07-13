#ifndef BLUES_CONF_H
#define BLUES_CONF_H

#include <Arduino.h>
#include <Wire.h>
#include <Notecard.h>
#include "WB_press_sensor.h"

// Add the Product UID gotten in Blues Notehub
#define PRODUCT_UID "com.company.username.projectname"//Change for your Product UID
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
//@brief function to read sensors and send data using Blues 
void send_data_using_blues(){

    J *req = notecard.newRequest("note.add");
    if (req != NULL) {
    JAddStringToObject(req, "file", "data.qo");
    Serial.println("Communication enabled");
    JAddBoolToObject(req, "sync", true);
    Serial.println("Sync Ready!");

    J *body = JCreateObject();
    if (body != NULL) {
      JAddNumberToObject(body, "temperature", read_bar().temp);
      JAddNumberToObject(body, "pressure", read_bar().press);
      JAddNumberToObject(body, "altitude", readAltitude(1013.25));
      JAddItemToObject(req, "body", body);
    }
    notecard.sendRequest(req);
    Serial.println("It worked!");
  }
}



#endif
