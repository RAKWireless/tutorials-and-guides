#ifndef GNSS_MODULE_H
#define GNSS_MODULE_H

#include <Wire.h> //Needed for I2C to GNSS
#include <SparkFun_u-blox_GNSS_Arduino_Library.h>

SFE_UBLOX_GNSS g_myGNSS;

long g_lastTime = 0; //Simple local timer. Limits amount if I2C traffic to u-blox module.
struct gnss_data{
	long d_lat, d_lon, d_alt, d_GrSpeed, d_heat;
  byte SVI;
};

// Function to init the gnss module
void config_gnss()
{
  pinMode(WB_IO2, OUTPUT);
  digitalWrite(WB_IO2, 0);
  delay(1000);
  digitalWrite(WB_IO2, 1);
  delay(1000);

  Wire.begin();
  // Initialize Serial for debug output
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
  Serial.println("GPS ZOE-M8Q Example(I2C)");
  if (g_myGNSS.begin() == false) //Connect to the u-blox module using Wire port
  {
    Serial.println(F("u-blox GNSS not detected at default I2C address. Please check wiring. Freezing."));
    while (1);
  }

  g_myGNSS.setI2COutput(COM_TYPE_UBX); //Set the I2C port to output UBX only (turn off NMEA noise)
  g_myGNSS.saveConfigSelective(VAL_CFG_SUBSEC_IOPORT); //Save (only) the communications port settings to flash and BBR
}

//Simple function to read gnss data
gnss_data read_gnss()
{
  gnss_data gnss_buffer;
  if (millis() - g_lastTime > 1000)
  {
    g_lastTime = millis(); //Update the timer
    
    gnss_buffer.d_lat = g_myGNSS.getLatitude();
    gnss_buffer.d_lon = g_myGNSS.getLongitude();
    gnss_buffer.d_alt = g_myGNSS.getAltitude();    
    gnss_buffer.d_GrSpeed = g_myGNSS.getGroundSpeed();
    gnss_buffer.d_heat = g_myGNSS.getHeading();
    gnss_buffer.SVI = g_myGNSS.getSIV();
  }
  return(gnss_buffer);
}

#endif