#ifndef BLUES_CONF_H
#define BLUES_CONF_H

#include <Arduino.h>
#include <Wire.h>
#include <Notecard.h>
#include "gnss_module.h"
#include "b_logic.h"

// Add the Product UID gotten in Blues Notehub
#define PRODUCT_UID ""com.company.username:projectname""
Notecard notecard;
// Define variables to measure
float latitude, longitude;
int floor_st, movement;
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
    latitude = read_gnss().d_lat;
    longitude = read_gnss().d_lon;
    movement = getMovData();
    floor_st = getFloor();

    J *req = notecard.newRequest("note.add");
    if (req != NULL) {
    JAddStringToObject(req, "file", "data.qo");
    Serial.println("Communication enabled");
    JAddBoolToObject(req, "sync", true);
    Serial.println("Sync Ready!");

    J *body = JCreateObject();
    if (body != NULL) {
      JAddNumberToObject(body, "latitude", latitude);
      JAddNumberToObject(body, "longitude", longitude);
      JAddNumberToObject(body, "floor", floor_st);
      JAddNumberToObject(body, "movement", movement);
      JAddItemToObject(req, "body", body);
    }
    notecard.sendRequest(req);
    Serial.println("It worked!");
  }
}



#endif
