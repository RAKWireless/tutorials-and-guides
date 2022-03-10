#include <Arduino.h>
#include <Adafruit_TinyUSB.h>

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  pinMode(LED_BLUE, OUTPUT);
  digitalWrite(LED_BLUE, HIGH);
}

void loop() {
  // put your main code here, to run repeatedly:
  digitalWrite(LED_BLUE, HIGH);
  delay(1000);
  digitalWrite(LED_BLUE, LOW);
  delay(1000);
  Serial.println("Hello! I'm Alive");
  }
