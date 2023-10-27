#ifndef BLUES_CONF_H
#define BLUES_CONF_H

#include <Arduino.h>
#include <Wire.h>
#include <Notecard.h>
#include "rak12039.h"

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
      JAddNumberToObject(body, "PPM10S", read_bar().ppm10_standard);
      JAddNumberToObject(body, "PPM25S", read_bar().ppm25_standard);
      JAddNumberToObject(body, "PPM100S", read_bar().ppm100_standard);
      JAddNumberToObject(body, "PPM10E", read_bar().ppm10_env);
      JAddNumberToObject(body, "PPM25E", read_bar().ppm25_env);
      JAddNumberToObject(body, "PPM100E", read_bar().ppm100_env);
      JAddNumberToObject(body, "PPM03", read_bar().ppm_03um);
      JAddNumberToObject(body, "PPM05", read_bar().ppm_05um);
      JAddNumberToObject(body, "PPM10", read_bar().ppm_10um);
      JAddNumberToObject(body, "PPM25", read_bar().ppm_25um);
      JAddNumberToObject(body, "PPM50", read_bar().ppm_50um);
      JAddNumberToObject(body, "PPM100", read_bar().ppm_100um);
         
    }
    notecard.sendRequest(req);
    Serial.println("It worked!");
  }
  readppm();   
}



#endif