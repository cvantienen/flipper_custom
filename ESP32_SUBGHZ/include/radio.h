#ifndef RADIO_CC1101_H
#define RADIO_CC1101_H

#include <Arduino.h>
#include <ELECHOUSE_CC1101_SRC_DRV.h>

// CC1101 pin definitions - kept with Radio class
#define RADIO_SCK_PIN   18
#define RADIO_MISO_PIN  19
#define RADIO_MOSI_PIN  23
#define RADIO_SS_PIN    5
#define RADIO_GDO0_PIN  2
#define RADIO_GDO2_PIN  4

class RadioCC1101 {
private:
    float currentFrequency;
    bool initialized;
    
public:
    RadioCC1101() : currentFrequency(433.92), initialized(false) {}
    
    bool begin(float mhz = 433.92) {
        Serial.println("[Radio] Initializing CC1101...");
        
        ELECHOUSE_cc1101.setSpiPin(RADIO_SCK_PIN, RADIO_MISO_PIN, RADIO_MOSI_PIN, RADIO_SS_PIN);
        ELECHOUSE_cc1101.setGDO(RADIO_GDO0_PIN, RADIO_GDO2_PIN);
        ELECHOUSE_cc1101.Init();
        
        ELECHOUSE_cc1101.setMHZ(mhz);
        ELECHOUSE_cc1101.SetTx();
        ELECHOUSE_cc1101.setModulation(2);
        ELECHOUSE_cc1101.setDRate(512);
        ELECHOUSE_cc1101.setPktFormat(3);
        
        currentFrequency = mhz;
        initialized = ELECHOUSE_cc1101.getCC1101();
        
        if (initialized) {
            Serial.printf("[Radio] ✅ Initialized @ %.2f MHz\n", mhz);
        } else {
            Serial.println("[Radio] ❌ Failed to initialize");
        }
        
        delay(50);
        return initialized;
    }
    
    void setFrequency(float mhz) {
        if (mhz != currentFrequency) {
            ELECHOUSE_cc1101.setMHZ(mhz);
            currentFrequency = mhz;
            Serial.printf("[Radio] Frequency changed to %.2f MHz\n", mhz);
        }
    }
    
    void transmit(int samples[], int length, float mhz) {
        if (!initialized) {
            Serial.println("[Radio] ❌ Not initialized!");
            return;
        }
        
        setFrequency(mhz);
        
        Serial.printf("[Radio] Transmitting %d samples @ %.2f MHz\n", length, mhz);
        
        for (int i = 0; i < length; i++) {
            int delayTime = samples[i];
            byte state = 1;
            
            if (delayTime < 0) {
                delayTime = -delayTime;
                state = 0;
            }
            
            digitalWrite(RADIO_GDO0_PIN, state);
            delayMicroseconds(delayTime);
            
            if (i % 100 == 0) {
                yield(); // Feed watchdog
            }
        }
        
        digitalWrite(RADIO_GDO0_PIN, 0);
        Serial.println("[Radio] ✅ Transmission complete");
        delay(20);
    }
    
    bool isReady() const {
        return initialized;
    }
    
    float getFrequency() const {
        return currentFrequency;
    }
};

#endif