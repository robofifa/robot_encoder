/* .
*/
#include "Encoder.h"

Encoder *Left;

const uint8_t CHAN_A = 16;
const uint8_t CHAN_B = 17;
const unsigned int FILTERLENGTH = 1023;

void setup() {
  Serial.begin(115200);
  /* Initialize PCNT event queue and PCNT functions */
  Left = new Encoder(PCNT_UNIT_0, CHAN_A, CHAN_B, Encoding::X4, FILTERLENGTH);
}

unsigned long count = 0;

void loop() {
  /* Wait for the event information passed from PCNT's interrupt handler.
   * Once received, decode the event type and print it on the serial monitor.
   */
  if(count < (millis() - 100)){
    count = millis();
    Serial.println(Left->ReadAndReset());
  }

}
