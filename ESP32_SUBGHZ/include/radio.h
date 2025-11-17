#ifndef RADIO_H
#define RADIO_H

// SAMPLE LENGTHS
#define LENGTH_SAMPLES_SIGNAL_1 5
#define LENGTH_SAMPLES_SIGNAL_2 5
#define LENGTH_SAMPLES_SIGNAL_3 5
#define LENGTH_SAMPLES_SIGNAL_4 5

// CC1101 PIN DEFINITIONS
// EBYTE E07-M1101D-SMA 433MHz  variation 
#define PIN_M1101_SCK 18
#define PIN_M1101_MISO 19
#define PIN_M1101_MOSI 23
#define PIN_M1101_SS 5
#define PIN_GDO0 2
#define PIN_GDO2 4

// FUNCTION DECLARATIONS
void initCC1101(float mhz);
void sendSamples(int samples[], int samplesLength, float mhz);

#endif
