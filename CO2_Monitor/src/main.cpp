#include <Arduino.h>
#include "Lorawan_code.h"
#include "blues_conf.h"
#include "CO2_sensor.h"

lmh_confirm success_send = LMH_UNCONFIRMED_MSG;	

static uint32_t count = 0;
static uint32_t count_fail = 0;
static uint32_t timers_init(void);

uint16_t temperature, humidity, CO2;
data_sensor data;

void setup() {
  setup_c02sensor();
  blues_card_conf();
  conf_lora();

  //create a user timer to send data to server period
  uint32_t err_code;
  err_code = timers_init();
  if (err_code != 0)
  {
    Serial.printf("timers_init failed - %d\n", err_code);
    return;
  }
}

void loop() {
  //@brief function to send data using Blues when there is an error in LoRa Connection
  if(not_LoRa_connection == true){
      sending();
      delay(3*60*1000);
  }
  delay(10000);
}
// Functions to send data via LoRaWAN
void send_lora_frame(void)
{
  data = read_data();
  temperature = data.temp*100;
  humidity = data.hum*100;
  CO2 = data.co2;

  if (lmh_join_status_get() != LMH_SET)
  {
    //Not joined, try again later
    return;
  }
  //Create the buffer to be sent
  uint32_t i = 0;
  memset(m_lora_app_data.buffer, 0, LORAWAN_APP_DATA_BUFF_SIZE);
  m_lora_app_data.port = gAppPort;
  m_lora_app_data.buffer[i++] = (uint8_t)(CO2 >> 8);
  m_lora_app_data.buffer[i++] = (uint8_t)(CO2);
  m_lora_app_data.buffer[i++] = (uint8_t)(temperature  >> 8);
  m_lora_app_data.buffer[i++] = (uint8_t)(temperature);
  m_lora_app_data.buffer[i++] = (uint8_t)(humidity  >> 8);
  m_lora_app_data.buffer[i++] = (uint8_t)(humidity);
  m_lora_app_data.buffsize = i;

  lmh_error_status error = lmh_send(&m_lora_app_data, success_send);
  if (error == LMH_SUCCESS)
  {
    count++;
    Serial.printf("lmh_send ok count %d\n", count);
  }
  else
  {
    count_fail++;
    Serial.printf("lmh_send fail count %d\n", count_fail);
    Serial.println("Sending data using Blues");
    not_LoRa_connection = true;
  }
}

/**@brief Function for handling user timerout event.
 */
void tx_lora_periodic_handler(void)
{
  TimerSetValue(&appTimer, LORAWAN_APP_INTERVAL);
  TimerStart(&appTimer);
  Serial.println("Sending frame now...");
  send_lora_frame();
}

/**@brief Function for the Timer initialization.
 *
 * @details Initializes the timer module. This creates and starts application timers.
 */
uint32_t timers_init(void)
{
  TimerInit(&appTimer, tx_lora_periodic_handler);
  return 0;
}


