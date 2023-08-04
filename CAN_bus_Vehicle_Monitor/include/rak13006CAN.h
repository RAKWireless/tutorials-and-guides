#ifndef RAK13006CAN_H
#define RAK13006CAN_H

#include <Rak_CAN.h>
float CAN_Data1, CAN_Data2, CAN_Data3, CAN_Data4, CAN_Data5, CAN_Data6, CAN_Data7, CAN_Data8;

struct bar_data
{
    float voltage;
    float rpm;
    float oilpressure;
};

void caninit()
{
    pinMode(WB_IO2, OUTPUT);
    digitalWrite(WB_IO2, HIGH); // Enable power supply.
    delay(300);
    Serial.begin(115200);
    while (!Serial)
    {
        delay(50);
    }
    Rak_Can.init();
    Serial.print("sizeof (RAK_CAN_Settings): ");
    Serial.print(sizeof(RAK_CAN_Settings));
    Serial.println(" bytes");
    Serial.println("Configure ACAN2518");
    RAK_CAN_Settings settings(RAK_CAN_Settings::OSC_40MHz, 500 * 1000); // CAN bit rate 125 kb/s
    settings.mRequestedMode = RAK_CAN_Settings::Normal20B;
    const uint32_t errorCode = Rak_Can.begin(settings, []
                                             { Rak_Can.isr(); });
    if (errorCode == 0)
    {
        Serial.print("Bit Rate prescaler: ");
        Serial.println(settings.mBitRatePrescaler);
        Serial.print("Phase segment 1: ");
        Serial.println(settings.mPhaseSegment1);
        Serial.print("Phase segment 2: ");
        Serial.println(settings.mPhaseSegment2);
        Serial.print("SJW:");
        Serial.println(settings.mSJW);
        Serial.print("Actual bit rate: ");
        Serial.print(settings.actualBitRate());
        Serial.println(" bit/s");
        Serial.print("Exact bit rate ? ");
        Serial.println(settings.exactBitRate() ? "yes" : "no");
        Serial.print("Sample point: ");
        Serial.print(settings.samplePointFromBitStart());
        Serial.println("%");
    }
    else
    {
        Serial.print("Configuration error 0x");
        Serial.println(errorCode, HEX);
    }
}

// static uint32_t gBlinkLedDate = 0 ;
static uint32_t gReceivedFrameCount = 0;

void readcan()
{
    CANMessage frame;

    {
        Rak_Can.isr_core();
        if (Rak_Can.receive(frame))
        {
            if (!frame.rtr && (!frame.ext) && (frame.id == 0xA))
            {
                gReceivedFrameCount++;
                Serial.print("Received: ");
                for (int i = 0; i < 8; i++)
                {
                    CAN_Data1 = frame.data[0];
                    CAN_Data2 = frame.data[1];
                    CAN_Data3 = frame.data[2];
                    CAN_Data4 = frame.data[3];
                    CAN_Data5 = frame.data[4];
                    CAN_Data6 = frame.data[5];
                    CAN_Data7 = frame.data[6];
                    CAN_Data8 = frame.data[7];
                }
                Serial.print("   gReceivedFrameCount:");
                Serial.println(gReceivedFrameCount);
                // Serial.println(frame.data[i]);
                    //Serial.print("VOLTAGE ");
                   // Serial.println(CAN_Data1);
                   // Serial.print("RPM ");
                  //  Serial.println(CAN_Data2);
                  //  Serial.print("OIL PRESSURE ");
                   // Serial.println(CAN_Data3);
            }
        }
    }
}

int returnCANdata1()
{
     return (CAN_Data1);
}
int returnCANdata2()
{
    return (CAN_Data2);
}
int returnCANdata3()
{
    return (CAN_Data3);
}

#endif