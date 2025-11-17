#include <ELECHOUSE_CC1101_SRC_DRV.h>
#include <radio.h>
//--------------------------------------------------------------------------------
// ---------------------------
// SAMPLE SIGNALS
// ---------------------------
int samples_signal_1[LENGTH_SAMPLES_SIGNAL_1] = {250, -250, 250, -250, 250};
int samples_signal_2[LENGTH_SAMPLES_SIGNAL_1] = {250, -250, 250, -250, 250};
int samples_signal_3[LENGTH_SAMPLES_SIGNAL_1] = {250, -250, 250, -250, 250};
int samples_signal_4[LENGTH_SAMPLES_SIGNAL_1] = {250, -250, 250, -250, 250};

// ---------------------------
// CC1101 INITIALIZATION
// ---------------------------
void initCC1101(float mhz)
{
  Serial.println("[initCC1101] Starting CC1101 init...");
  ELECHOUSE_cc1101.setSpiPin(PIN_M1101_SCK, PIN_M1101_MISO, PIN_M1101_MOSI, PIN_M1101_SS); // SCK, MISO, MOSI, SS
  ELECHOUSE_cc1101.setGDO(PIN_GDO0, PIN_GDO2);

  ELECHOUSE_cc1101.Init();

  ELECHOUSE_cc1101.setMHZ(mhz);
  ELECHOUSE_cc1101.SetTx();
  ELECHOUSE_cc1101.setModulation(2);
  ELECHOUSE_cc1101.setDRate(512);
  ELECHOUSE_cc1101.setPktFormat(3);

  if (!ELECHOUSE_cc1101.getCC1101())
  {
    Serial.println("[initCC1101] ❌ CC1101 Connection Error!");
  }
  else
  {
    Serial.print("[initCC1101] ✅ CC1101 Initialized OK @ ");
    Serial.print(mhz);
    Serial.println(" MHz");
  }

  delay(50); // prevent brownout
}

// ---------------------------
// SEND SAMPLES
// ---------------------------
void sendSamples(int samples[], int samplesLenght, float mhz)
{

  initCC1101(mhz);
  Serial.print("[sendSamples] Transmitting ");
  Serial.print(samplesLenght);
  int delayTime = 0;
  byte n = 0;

  for (int i = 0; i < samplesLenght; i++)
  {
    delayTime = samples[i];
    n = 1;
    if (delayTime < 0)
    {
      delayTime = -delayTime;
      n = 0;
    }

    digitalWrite(PIN_GDO0, n);
    delayMicroseconds(delayTime);

    if (i % 100 == 0)
    {
      Serial.print("."); // progress indicator
      yield();           // feed watchdog
    }
  }

  digitalWrite(PIN_GDO0, 0);
  Serial.println("\n[sendSamples] ✅ Transmission completed");
  delay(20);
}

// // ---------------------------
// // OLD MAIN LOOP
// // ---------------------------
// // void loop() {
// //   yield(); // always feed watchdog

// //   // BUTTON 1
// //   if (digitalRead(PIN_BTN_1) == LOW) {
// //       Serial.println("[loop] Button 1 pressed");
// //       while (digitalRead(PIN_BTN_1) == LOW) { delay(10); yield(); }
// //       sendSamples(samples_signal_1, LENGTH_SAMPLES_SIGNAL_1, 433.92);
// //   }

// //   // BUTTON 2
// //   if (digitalRead(PIN_BTN_2) == LOW) {
// //       Serial.println("[loop] Button 2 pressed");
// //       while (digitalRead(PIN_BTN_2) == LOW) { delay(10); yield(); }
// //       sendSamples(samples_signal_2, LENGTH_SAMPLES_SIGNAL_2, 315);
// //   }

// //   // BUTTON 3
// //   if (digitalRead(PIN_BTN_3) == LOW) {
// //       Serial.println("[loop] Button 3 pressed");
// //       while (digitalRead(PIN_BTN_3) == LOW) { delay(10); yield(); }
// //       sendSamples(samples_signal_3, LENGTH_SAMPLES_SIGNAL_3, 433.92);
// //   }

// //   // BUTTON 4
// //   if (digitalRead(PIN_BTN_4) == LOW) {
// //       Serial.println("[loop] Button 4 pressed");
// //       while (digitalRead(PIN_BTN_4) == LOW) { delay(10); yield(); }
// //       sendSamples(samples_signal_4, LENGTH_SAMPLES_SIGNAL_4, 433.92);
// //   }

// //   delay(50); // stability
// // }