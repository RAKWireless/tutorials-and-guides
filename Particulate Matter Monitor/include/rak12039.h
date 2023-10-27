#ifndef RAK12039_H
#define RAK12039_H

#include "RAK12039_PMSA003I.h"

RAK_PMSA003I PMSA003I;

/*
 * @brief WB_IO6 is connected to the SET pin.
 *        Set pin/TTL level @3.3V, high level or suspending is normal working status.
 *        while low level is sleeping mode.
 */
#define SET_PIN   WB_IO6

struct bar_data
{
    float ppm10_standard;
    float ppm25_standard;
    float ppm100_standard;
    float ppm10_env;
    float ppm25_env;
    float ppm100_env;
    float ppm_03um;
    float ppm_05um;
    float ppm_10um;
    float ppm_25um;
    float ppm_50um;
    float ppm_100um;
    
};



void rak12039init() 
{
  // Sensor power on.
  pinMode(WB_IO2, OUTPUT);
  digitalWrite(WB_IO2, HIGH); 

  pinMode(SET_PIN, OUTPUT);
  digitalWrite(SET_PIN, HIGH);  
   
  // Initialize Serial for debug output.
  time_t timeout = millis();
  Serial.begin(115200);
  while (!Serial)
  {
    if ((millis() - timeout) < 5000)
    {
      delay(100);
    }
    else
    {
      break;
    }
  }

  Wire.begin();
  if(!PMSA003I.begin()) 
  {
    Serial.println("PMSA003I begin fail,please check connection!");
    delay(100);
    while(1);
  }
}

void readppm() 
{
  PMSA_Data_t data;
  
  if (PMSA003I.readDate(&data)) 
  {
    Serial.println("PMSA003I read date success.");

    Serial.println("Standard particulate matter:");
    Serial.print("PM1.0: "); 
    Serial.print(data.pm10_standard);
    Serial.println(" [µg/𝑚3]"); 
    
    Serial.print("PM2.5: "); 
    Serial.print(data.pm25_standard);
    Serial.println(" [µg/𝑚3]"); 
    
    Serial.print("PM10 : "); 
    Serial.print(data.pm100_standard);
    Serial.println(" [µg/𝑚3]"); 

    Serial.println("Atmospheric environment:");
    Serial.print("PM1.0: "); 
    Serial.print(data.pm10_env);
    Serial.println(" [µg/𝑚3]");
     
    Serial.print("PM2.5: "); 
    Serial.print(data.pm25_env);
    Serial.println(" [µg/𝑚3]"); 
    
    Serial.print("PM10 : "); 
    Serial.print(data.pm100_env);
    Serial.println(" [µg/𝑚3]");

    Serial.println("The number of particles in 0.1L air (above diameter):");
    Serial.print("0.3um:"); 
    Serial.println(data.particles_03um);
    Serial.print("0.5um:"); 
    Serial.println(data.particles_05um);
    Serial.print("1.0um:"); 
    Serial.println(data.particles_10um);
    Serial.print("2.5um:"); 
    Serial.println(data.particles_25um);
    Serial.print("5.0um:"); 
    Serial.println(data.particles_50um);
    Serial.print("10 um:"); 
    Serial.println(data.particles_100um);
  }
  else
  {
    Serial.println("PMSA003I read failed!");
  }
  Serial.println();
  delay(1000);
}


bar_data read_bar() {
    PMSA_Data_t data;
    if (PMSA003I.readDate(&data)) 
  {
  bar_data sensor_reading;
  sensor_reading.ppm10_standard = data.pm10_standard;
  sensor_reading.ppm25_standard = data.pm25_standard;
  sensor_reading.ppm100_standard = data.pm100_standard;
  sensor_reading.ppm10_env = data.pm10_env;
  sensor_reading.ppm25_env = data.pm25_env;
  sensor_reading.ppm100_env = data.pm100_env;
  sensor_reading.ppm_03um = data.particles_03um;
  sensor_reading.ppm_05um = data.particles_05um;
  sensor_reading.ppm_10um = data.particles_10um;
  sensor_reading.ppm_25um = data.particles_25um;
  sensor_reading.ppm_50um = data.particles_50um;
  sensor_reading.ppm_100um = data.particles_100um;
  return(sensor_reading);
  }
   
  
}

#endif