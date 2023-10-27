#include <Arduino.h>
#include "Lorawan_code.h"
#include "blues_conf.h"
#include "rak12039.h"

lmh_confirm success_send = LMH_UNCONFIRMED_MSG;

static uint32_t count = 0;
static uint32_t count_fail = 0;
static uint32_t timers_init(void);

uint32_t ppm10s, ppm25s, ppm100s, ppm10e, ppm25e, ppm100e, ppm03, ppm05, ppm10, ppm25, ppm50, ppm100;

void setup()
{
  // conf_bar();
  blues_card_conf();
  conf_lora();
  rak12039init();
  // create a user timer to send data to server period
  uint32_t err_code;
  err_code = timers_init();
  if (err_code != 0)
  {
    Serial.printf("timers_init failed - %d\n", err_code);
    return;
  }
}

void loop()
{ read_bar();
  // Read sensors data each 2 seconds
   ppm10s = read_bar().ppm10_standard;
   ppm25s = read_bar().ppm25_standard;
   ppm100s = read_bar().ppm100_standard;
   ppm10e = read_bar().ppm10_env;
   ppm25e = read_bar().ppm25_env;
   ppm100e = read_bar().ppm100_env;
   ppm03 = read_bar().ppm_03um;
   ppm05 = read_bar().ppm_05um;
   ppm10 = read_bar().ppm_10um;
   ppm25 = read_bar().ppm_25um;
   ppm50 = read_bar().ppm_50um;
   ppm100 = read_bar().ppm_100um;

 Serial.printf("ppm03: %d, ppm05: %d, \n", ppm03, ppm05);
  
  //@brief function to send data using Blues when there is an error in LoRa Connection
  if (not_LoRa_connection == true)
  {
    send_data_using_blues();
    delay(15000);
  }
  delay(2000);
}
// Functions to send data via LoRaWAN
void send_lora_frame(void)
{
  
  if (lmh_join_status_get() != LMH_SET)
  {
    // Not joined, try again later
    return;
  }
  // Create the buffer to be sent
  uint32_t i = 0;
  memset(m_lora_app_data.buffer, 0, LORAWAN_APP_DATA_BUFF_SIZE);
  m_lora_app_data.port = gAppPort;
  m_lora_app_data.buffer[i++] = 0x01;
  m_lora_app_data.buffer[i++] = (uint8_t)(ppm10s >> 8);
  m_lora_app_data.buffer[i++] = (uint8_t)(ppm10s);
  m_lora_app_data.buffer[i++] = (uint8_t)(ppm25s >> 8);
  m_lora_app_data.buffer[i++] = (uint8_t)(ppm25s);
  m_lora_app_data.buffer[i++] = (uint8_t)(ppm100s >> 8);
  m_lora_app_data.buffer[i++] = (uint8_t)(ppm100s);
  m_lora_app_data.buffer[i++] = (uint8_t)(ppm10e >> 8);
  m_lora_app_data.buffer[i++] = (uint8_t)(ppm10e);
  m_lora_app_data.buffer[i++] = (uint8_t)(ppm25e >> 8);
  m_lora_app_data.buffer[i++] = (uint8_t)(ppm25e);
  m_lora_app_data.buffer[i++] = (uint8_t)(ppm100e >> 8);
  m_lora_app_data.buffer[i++] = (uint8_t)(ppm100e);
  m_lora_app_data.buffer[i++] = (uint8_t)(ppm03 >> 8);
  m_lora_app_data.buffer[i++] = (uint8_t)(ppm03);
  m_lora_app_data.buffer[i++] = (uint8_t)(ppm05 >> 8);
  m_lora_app_data.buffer[i++] = (uint8_t)(ppm05);
  m_lora_app_data.buffer[i++] = (uint8_t)(ppm10 >> 8);
  m_lora_app_data.buffer[i++] = (uint8_t)(ppm10);
  m_lora_app_data.buffer[i++] = (uint8_t)(ppm25 >> 8);
  m_lora_app_data.buffer[i++] = (uint8_t)(ppm25);
  m_lora_app_data.buffer[i++] = (uint8_t)(ppm50 >> 8);
  m_lora_app_data.buffer[i++] = (uint8_t)(ppm50);
  m_lora_app_data.buffer[i++] = (uint8_t)(ppm100 >> 8);
  m_lora_app_data.buffer[i++] = (uint8_t)(ppm100);
  m_lora_app_data.buffsize = i;
  readppm();
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