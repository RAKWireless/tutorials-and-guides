#ifndef NFCREADER_H
#define NFCREADER_H

#include <Wire.h>
#include <SPI.h>
#include <RAK13600-PN532.h>  // Click here to get the library: http://librarymanager/All#RAK13600-PN532

// If using the breakout or shield with I2C, define just the pins connected
#define PN532_IRQ (WB_IO6)
#define PN532_RESET (WB_IO5)  // Not connected by default on the NFC Shield

// Or use this line for a breakout or shield with an I2C connection:
NFC_PN532 nfc(PN532_IRQ, PN532_RESET);

void nfcstarscan() {
  boolean success;
  uint8_t uid[] = { 0, 0, 0, 0, 0, 0, 0 };  // Buffer to store the returned UID
  uint8_t uidLength;                        // Length of the UID (4 or 7 bytes depending on ISO14443A card type)
  // Wait for an ISO14443B type cards (Mifare, etc.).  When one is found
  // 'uid' will be populated with the UID, and uidLength will indicate
  // if the uid is 4 bytes (Mifare Classic) or 7 bytes (Mifare Ultralight)
  success = nfc.readPassiveTargetID(PN532_MIFARE_ISO14443A, &uid[0], &uidLength);

  if (success) {
    Serial.println("Found a card!");
    Serial.print("UID Length: ");
    Serial.print(uidLength, DEC);
    Serial.println(" bytes");
    Serial.print("UID Value: ");
    for (uint8_t i = 0; i < uidLength; i++) {
      Serial.print(" 0x");
      Serial.print(uid[i], HEX);
    }
    Serial.println("");
    // Wait 1 second before continuing
    delay(1000);
  } else {
    // PN532 probably timed out waiting for a card
    Serial.println("Timed out waiting for a card");
  }
  
}
#endif