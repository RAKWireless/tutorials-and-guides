#ifndef BLUES_CONF_H
#define BLUES_CONF_H

#include <Arduino.h>
#include <Wire.h>
#include <Notecard.h>
#include "rak13006CAN.h"

// Add the Product UID gotten in Blues Notehub
#define PRODUCT_UID "com.gmail.ingharoldduarte:motor_monitor"
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
      JAddNumberToObject(body, "CANvoltage", returnCANdata1());
      JAddNumberToObject(body, "CANrpm", returnCANdata2());
      JAddNumberToObject(body, "CANoilpressure", returnCANdata3());
      JAddItemToObject(req, "body", body);
      Serial.print("VOLTAGE ");
                    Serial.println(CAN_Data1);
                    Serial.print("RPM ");
                    Serial.println(CAN_Data2);
                    Serial.print("OIL PRESSURE ");
                    Serial.println(CAN_Data3);
    }
    notecard.sendRequest(req);
    Serial.println("It worked!");
  }
}



#endif
