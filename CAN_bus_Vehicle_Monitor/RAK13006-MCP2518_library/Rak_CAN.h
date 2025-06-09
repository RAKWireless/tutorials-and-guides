#ifndef RAK_CAN_H
#define RAK_CAN_H

#include "ACAN2517.h"
#include "ACAN2517Settings.h"

class RAK_CAN : public ACAN2517
{
public:
  RAK_CAN(const uint8_t inCS, SPIClass &inSPI, const uint8_t inINT) : ACAN2517(inCS, inSPI, inINT){};
  void init(void);
  ~RAK_CAN(){};
};

class RAK_CAN_Settings : public ACAN2517Settings
{
public:
  RAK_CAN_Settings(const Oscillator inOscillator, const uint32_t inDesiredBitRate, const uint32_t inTolerancePPM = 1000) : ACAN2517Settings(inOscillator, inDesiredBitRate, inTolerancePPM){};
  ~RAK_CAN_Settings(){};
};

extern RAK_CAN Rak_Can;
#endif
