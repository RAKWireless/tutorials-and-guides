/**
 * @file Home.ino
 * @author rakwireless.com
 * @Easy transceiver for LoRa test using WisBlock
 * @version 0.1
 * @date 2022-02-25
 * 
 * @copyright Copyright (c) 2022
 * 
 * @note RAK4631 GPIO mapping to nRF52840 GPIO ports
   RAK4631    <->  nRF52840
   WB_IO1     <->  P0.17 (GPIO 17)
   WB_IO2     <->  P1.02 (GPIO 34)
   WB_IO3     <->  P0.21 (GPIO 21)
   WB_IO4     <->  P0.04 (GPIO 4)
   WB_IO5     <->  P0.09 (GPIO 9)
   WB_IO6     <->  P0.10 (GPIO 10)
   WB_SW1     <->  P0.01 (GPIO 1)
   WB_A0      <->  P0.04/AIN2 (AnalogIn A2)
   WB_A1      <->  P0.31/AIN7 (AnalogIn A7)
 */
#include <Arduino.h>
#include <Adafruit_TinyUSB.h>
#include <SX126x-RAK4630.h> //http://librarymanager/All#SX126x
#include <SPI.h>

// Function declarations
void OnRxDone(uint8_t *payload, uint16_t size, int16_t rssi, int8_t snr);
void OnRxTimeout(void);
void OnRxError(void);
void OnTxDone(void);
void OnTxTimeout(void);
void OnCadDone(bool cadResult);

// Define LoRa parameters
#define RF_FREQUENCY 915000000	// Hz
#define TX_OUTPUT_POWER 22		// dBm
#define LORA_BANDWIDTH 0		// [0: 125 kHz, 1: 250 kHz, 2: 500 kHz, 3: Reserved]
#define LORA_SPREADING_FACTOR 7 // [SF7..SF12]
#define LORA_CODINGRATE 1		// [1: 4/5, 2: 4/6,  3: 4/7,  4: 4/8]
#define LORA_PREAMBLE_LENGTH 8	// Same for Tx and Rx
#define LORA_SYMBOL_TIMEOUT 0	// Symbols
#define LORA_FIX_LENGTH_PAYLOAD_ON false
#define LORA_IQ_INVERSION_ON false
#define RX_TIMEOUT_VALUE 3000
#define TX_TIMEOUT_VALUE 3000

static RadioEvents_t RadioEvents;

static uint8_t RcvBuffer[64];
static uint8_t TxdBuffer[64];

void setup()
{

	// Initialize LoRa chip.
	lora_rak4630_init();

  // Initialize LEDs.
  pinMode(LED_BLUE, OUTPUT);
  digitalWrite(LED_BLUE, LOW);
  pinMode(LED_GREEN, OUTPUT);
  digitalWrite(LED_GREEN, LOW);
	
	Serial.println("=====================================");
	Serial.println("Home Saying Hello!");
	Serial.println("=====================================");

	// Initialize the Radio callbacks
	RadioEvents.TxDone = OnTxDone;
	RadioEvents.RxDone = OnRxDone;
	RadioEvents.TxTimeout = OnTxTimeout;
	RadioEvents.RxTimeout = OnRxTimeout;
	RadioEvents.RxError = OnRxError;
	RadioEvents.CadDone = OnCadDone;

	// Initialize the Radio
	Radio.Init(&RadioEvents);

	// Set Radio channel
	Radio.SetChannel(RF_FREQUENCY);

	// Set Radio RX configuration
	Radio.SetRxConfig(MODEM_LORA, LORA_BANDWIDTH, LORA_SPREADING_FACTOR,
					  LORA_CODINGRATE, 0, LORA_PREAMBLE_LENGTH,
					  LORA_SYMBOL_TIMEOUT, LORA_FIX_LENGTH_PAYLOAD_ON,
					  0, true, 0, 0, LORA_IQ_INVERSION_ON, true);

  // Set Radio TX configuration
  Radio.SetTxConfig(MODEM_LORA, TX_OUTPUT_POWER, 0, LORA_BANDWIDTH,
            LORA_SPREADING_FACTOR, LORA_CODINGRATE,
            LORA_PREAMBLE_LENGTH, LORA_FIX_LENGTH_PAYLOAD_ON,
            true, 0, 0, LORA_IQ_INVERSION_ON, TX_TIMEOUT_VALUE);

	// Start LoRa
	Serial.println("Starting Radio.Rx");
	send();
 
}

void loop()
{
 // Put your application tasks here, like reading of sensors,
  // Controlling actuators and/or other functions. 

}

/**@brief Function to be executed on Radio Rx Done event
 */
void OnRxDone(uint8_t *payload, uint16_t size, int16_t rssi, int8_t snr)
{
  digitalWrite(LED_BLUE, HIGH);
	Serial.println("OnRxDone");
	delay(100);
 digitalWrite(LED_BLUE, LOW);
	memcpy(RcvBuffer, payload, size);

	Serial.printf("RssiValue=%d dBm, SnrValue=%d\n", rssi, snr);

	for (int idx = 0; idx < size; idx++)
	{
		Serial.print(RcvBuffer[idx], HEX);
	}
	Serial.println("");

  delay(100);
  Radio.Standby();
  Radio.SetCadParams(LORA_CAD_08_SYMBOL, LORA_SPREADING_FACTOR + 13, 10, LORA_CAD_ONLY, 0);
  Radio.StartCad();
 
}

/**@brief Function to be executed on Radio Rx Timeout event
 */
void OnRxTimeout(void)
{
	Serial.println("OnRxTimeout");
	Radio.Rx(RX_TIMEOUT_VALUE);
}

/**@brief Function to be executed on Radio Rx Error event
 */
void OnRxError(void)
{
	Serial.println("OnRxError");
	Radio.Rx(RX_TIMEOUT_VALUE);
}

void OnTxDone(void){
  digitalWrite(LED_GREEN, HIGH);
  Serial.println("OnTxDone");
  delay(100);
  digitalWrite(LED_GREEN, LOW);  
  Radio.Rx(RX_TIMEOUT_VALUE);  
}

void OnTxTimeout(void){
  Serial.println("OnTxTimeout");
  Radio.Rx(RX_TIMEOUT_VALUE);
  
}
void OnCadDone(bool cadResult){
  if(cadResult){
    Serial.println("Buffer bussy");
    delay(2000);
    Radio.Rx(RX_TIMEOUT_VALUE);
  }
  else{
    Serial.println("Channel Free, I am sending data");  
    delay (100);
    send();
  }
  
  
}
void send()
{
  TxdBuffer[0] = 'H';
  TxdBuffer[1] = 'i';
  TxdBuffer[2] = '!';
  Radio.Send(TxdBuffer, 3);
  
}
