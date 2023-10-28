#ifndef BLUES_CONF_H
#define BLUES_CONF_H

#include <Arduino.h>
#include <Notecard.h>
#include "CO2_sensor.h"

// Add the Product UID gotten in Blues Notehub
#define PRODUCT_UID ""
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
void sending() {
  data_sensor sensorVars;
  sensorVars = read_data();
  J *req = notecard.newRequest("note.add");
  if (req != NULL) {
    JAddStringToObject(req, "file", "data.qo");
    Serial.println("Communication enabled");
    JAddBoolToObject(req, "sync", true);
    Serial.println("Sync Ready!");

    J *body = JCreateObject();
    if (body != NULL) {
      JAddNumberToObject(body, "temp", sensorVars.temp);
      JAddNumberToObject(body, "hum", sensorVars.hum);
      JAddNumberToObject(body, "co2", sensorVars.co2);
      JAddItemToObject(req, "body", body);
    }
    notecard.sendRequest(req);
  }
}


#endif
