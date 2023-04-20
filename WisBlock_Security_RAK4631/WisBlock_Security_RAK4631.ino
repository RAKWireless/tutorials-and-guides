

#include "FINGERPRINTREADER.h"
#include "NFCREADER.h"

void setup() {


  //not all boards support all baud rates
  //check your board's documentation for more info
  Serial.begin(57600);
  time_t timeout = millis();
  while (!Serial) {
    if ((millis() - timeout) < 5000) {
      delay(100);
    } else {
      break;
    }
  }


  pinMode(WB_IO2, OUTPUT);
  digitalWrite(WB_IO2, HIGH);  //power on for Finger detection power

  //NFC part
  nfc.begin();
  uint32_t versiondata = nfc.getFirmwareVersion();
  if (!versiondata) {
    Serial.print("Didn't find PN53x board");
    while (1)
      ;  // halt
  }
  // Got ok data, print it out!
  Serial.print("Found chip PN5");
  Serial.println((versiondata >> 24) & 0xFF, HEX);
  Serial.print("Firmware ver. ");
  Serial.print((versiondata >> 16) & 0xFF, DEC);
  Serial.print('.');
  Serial.println((versiondata >> 8) & 0xFF, DEC);
  // Set the max number of retry attempts to read from a card
  // This prevents us from waiting forever for a card, which is
  // the default behaviour of the PN532.
  nfc.setPassiveActivationRetries(0xFF);

  //configure board to read RFID tags
  nfc.SAMConfig();

  Serial.println("Waiting for an ISO14443A card");

  
  r307_fps.begin(57600);
  pinMode(R307_TOUCH, INPUT_PULLUP);
  Serial.println();
  Serial.println("R30X Fingerprint Example Sketch");
  Serial.println("===============================");
  Serial.println("All commands and parameters must be separated by single whitespace.");
  Serial.println();

  //r307_fps.portControl(0);
  //you need to verify the password before you can do anything else
  Serial.print(F("Verifying password 0x"));
  Serial.println(R307_PASSWORD, HEX);
  uint8_t response = r307_fps.verifyPassword(R307_PASSWORD);
  if (response == 0) {
#if !defined(FPS_DEBUG)
    Serial.println(F("Successful\n"));
#else
    Serial.println();
#endif
  } else {
    Serial.println(F("Failed. Check your password. Otherwise try with default one.\n"));
  }
  print_help();

  //this is optional
  // Serial.println(F("Setting new address.."));
  // response = r307_fps.setAddress(0xFFFFFFFF);
  // Serial.println();
}

void loop() {
  
  startscanFP();
  nfcstarscan();
 
}