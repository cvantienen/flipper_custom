#ifndef MAIN_H    // start guard
#define MAIN_H

// M1101
#define PIN_GDO0 2
#define PIN_GDO2 4


#define PIN_LED_ONBOARD 2

// SAMPLE LENGTHS
#define LENGTH_SAMPLES_SIGNAL_1 5
#define LENGTH_SAMPLES_SIGNAL_2 5
#define LENGTH_SAMPLES_SIGNAL_3 5
#define LENGTH_SAMPLES_SIGNAL_4 5

// FUNCTION DECLARATIONS
void initCC1101(float mhz);
void sendSamples(int samples[], int samplesLength, float mhz);

#endif // MAIN_H  // end guard
