/* .
*/
#include "encoder.h"

Encoder *Left;

void setup() {
  Serial.begin(115200);
  /* Initialize PCNT event queue and PCNT functions */
  Left = new Encoder(PCNT_UNIT_0, 16, 17, Encoding::X4, 1023);
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
