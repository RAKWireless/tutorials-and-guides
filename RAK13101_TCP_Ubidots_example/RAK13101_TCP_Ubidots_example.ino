/**
   @file RAK13101_TCP_Ubidots_example.ino
   @author harold.duarte@rakwireless.com
   @brief example of the use of the RAK13101 with the TCP protocol and the ubidots platform
   @version 0.1
   @date 2022-07-19
   @copyright Copyright (c) 2022
**/
#include <Wire.h>
#define POWER_KEY WB_IO5 //powerkey for the GPRS module
#include <Adafruit_Sensor.h> //RAK1906 
#include <Adafruit_BME680.h> //RAK1906 
Adafruit_BME680 bme;
// Might need adjustments
#define SEALEVELPRESSURE_HPA (1008.76) //Barometric Pressure in mbar

void bme680_init()
{
  Wire.begin();

  if (!bme.begin(0x76)) {
    Serial.println("Could not find a valid BME680 sensor, check wiring!");
    return;
  }

  // Set up oversampling and filter initialization
  bme.setTemperatureOversampling(BME680_OS_8X);
  bme.setHumidityOversampling(BME680_OS_2X);
  bme.setPressureOversampling(BME680_OS_4X);
  bme.setIIRFilterSize(BME680_FILTER_SIZE_3);
  bme.setGasHeater(320, 150); // 320*C for 150 ms
}


void setup() {

  Serial.println("MC20 power up!");
  Serial1.begin(115200); // GPRS shield baud rate
  Serial.begin(115200); //RAK4631 BR
  delay(100);

  // Initialize the built in LED
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW);

  time_t timeout = millis();
  bool moduleSleeps = true;
  if (moduleSleeps)
  {
    // Module slept, wake it up
    pinMode(POWER_KEY, OUTPUT);
    digitalWrite(POWER_KEY, 0);
    delay(200);
    digitalWrite(POWER_KEY, 1);
    delay(2000);
    digitalWrite(POWER_KEY, 0);
    delay(1000);
    while (!Serial)
    {
      if ((millis() - timeout) < 5000)
      {
        delay(100);
        digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
      }
      else
      {
        break;
      }
    }
    bme680_init();

  }
}

void loop() {

  if (! bme.performReading()) {
    Serial.println("Failed to perform reading :(");
    return;
  }

  delay(15000); //waits 15 seconds to send data again
  tcp();
}


void tcp()
{
  Serial1.println("AT+QIMODE=0");  //Use AT+QIMODE command to select TCPIP Stack mode, it is non-transparent mode when AT+QIMODE=0, and AT+QIMODE=1 is transparent
  delay(200);
  ShowSerialData();
  Serial1.println("AT+QICSGP=1,\"internet.movistar.com.co\",\"movistar\",\"movistar\",0"); //Use AT+QICSGP=1,internet.movistar.com.co,movistar,movistar,0 to set APN as internet.movistar.com.co,user name as movistar,password as movistar
  delay(500);
  ShowSerialData();
  Serial1.println("AT+QIREGAPP");  //Start TCPIP task
  delay(200);
  ShowSerialData();
  Serial1.println("AT+QICSGP?");  //Check the current connecting mode(1: GPRS connecting mode£¬0: CSD connecting mode)
  delay(200);
  ShowSerialData();
  Serial1.println("AT+QIACT");  //The current connecting mode is GPRS connecting mode
  delay(200);
  ShowSerialData();
  Serial1.println("AT+QILOCIP");  //Get the local IP address
  delay(400);
  ShowSerialData();
  Serial1.println("ATV1");  //Use ATV1 to set the response format
  delay(200);
  ShowSerialData();
  Serial1.println("AT+QIHEAD=1");  //Use AT+QIHEAD=1 to add the header information when receive data
  delay(200);
  ShowSerialData();
  Serial1.println("AT+QIDNSIP=1");  //Use AT+QIDNSIP=0 to use the IP address to establish TCP/UDP session, while AT+QIDNSIP=1 is use the domain name to establish TCP/UDP session
  delay(200);
  ShowSerialData();
  Serial1.println("AT+QIOPEN=\"TCP\",\"industrial.api.ubidots.com\",\"9012\"");  //Use AT+QIOPEN=TCP,industrial.api.ubidots.com,9012 to connect to a TCP server (IP address: industrial.api.ubidots.com:9012) If return CONNECT OK means successfully connected to the remote server
  delay(5000);
  ShowSerialData();
  Serial1.println("AT+QISEND");  //AT+QISEND, send data to server, ">" from the UART indicates the following input data is considered as data to be send. After receiving ">", input data (TEST), the maximum length of the data is 1460, the data beyond 1460 will be omitted. Then use <CTRL+Z> to send data. When receive SEND OK means the data has been sent
  delay(2000);
  ShowSerialData();
  Serial1.println(">");
  delay(200);
  ShowSerialData();
  double temp = bme.temperature;
  double hum = bme.humidity;
  double pres = bme.pressure / 100.0;
  double co2 = bme.gas_resistance / 1000.0;
  String token = "BBFF-5mRERLy7jMPS9G0vHoAoTIoPz99IEQ"; //Default token from your ubidots account
  String device_name = "RAK13101"; //your device name on the ubidots platform 
  String StrThree = "ubidots/1.0|POST|" + token + "|" + device_name + "=>Temperature:" + temp + ",Humidity:" + hum + ",Pressure:" + pres + ",Gas:" + co2 + "|end" ;
  Serial1.println(StrThree);//send message
  delay(1000);
  ShowSerialData();
  Serial1.println((char)26); // Ctrl+z
  Serial1.println();
  delay(200);
  ShowSerialData();
  Serial1.println("AT+QICLOSE"); //Use AT+QICLOSE to close the connecting of TCP/UDP
  delay(1000);
  ShowSerialData();
  Serial1.println("AT+QIDEACT"); //Use AT+QIDEACT to deactivate GPRS context
  delay(1000);
  ShowSerialData();

}


void ShowSerialData()// if you want to see the AT commands on the Serial Monitor
{
  while (Serial1.available() != 0)
    Serial.write(Serial1.read());
}
