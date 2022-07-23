#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME680.h> // Click to install library: http://librarymanager/All#Adafruit_BME680

Adafruit_BME680 bme;
#define SEALEVELPRESSURE_HPA (1010.0) // Might need adjustments

// Network and Server details
#define WIFI_SSID "Your_Network_Name"
#define WIFI_PASSWORD "Your_Password"
#define SERVER "https://api.thingspeak.com/update?"
#define API_KEY "YOUR_API_KEY"

//Variables
float temp, pres, humi;

//Function to configure sensor
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
// Function to read sensor variables
void read_sensor(){
  if (! bme.performReading()) {
    Serial.println("Failed to perform reading :(");
    return;
  }
  temp = bme.temperature,
  Serial.print("Temperature = ");
  Serial.print(temp);
  Serial.println(" °C");

  pres = bme.pressure/100.0;
  Serial.print("Pressure = ");
  Serial.print(pres);
  Serial.println(" hPa");

  humi = bme.humidity;
  Serial.print("Humidity = ");
  Serial.print(humi);
  Serial.println(" %");
}
//Simple function to send AT commands 
void execute_at(char *at, char *expect = NULL, int32_t timeout = 1000)
{
    String resp = "";
    Serial1.write(at);
    delay(10);

    while (timeout--)
    {
        if (Serial1.available())
        {
            resp += Serial1.readString();
        }
        delay(1);
    }

    Serial.println(resp);
    if (expect != NULL)
    {
        if (resp.indexOf(expect) != -1)
        {
            Serial.println("Execute OK.");
        }
        else
        {
            Serial.println("Execute Fail.");
        }
    }
    resp = "";
}

void setup() {

  char cmd[256] = "";
  // Initialize the built in LED
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW);
  delay(1000);

  // Initialize Serial for debug output
  Serial.begin(115200);
  bme680_init();//Initialize sensor
  Serial1.begin(115200);// Initialize Serial to communicate with the RAK2305
  delay(1000);
  // Set RAK2305 as AP and STA role
  execute_at("AT+CWMODE=3\r\n", "OK");

  // Connect AP with ssid and password
  snprintf(cmd, sizeof(cmd), "AT+CWJAP=\"%s\",\"%s\"\r\n", WIFI_SSID, WIFI_PASSWORD);
  execute_at(cmd, "OK");
}

void loop() {
  char cmd[128] = "";
  
  read_sensor();
    snprintf(cmd, sizeof(cmd), "AT+HTTPCLIENT=2,0,\"%sapi_key=%s&field1=%.2f&field2=%.2f&field3=%.2f\",,,2\r\n", SERVER, API_KEY, temp, pres, humi);
    execute_at(cmd,"OK");
    delay(100000);
}
