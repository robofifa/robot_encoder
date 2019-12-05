/* Pulse counter module - Example
   For other examples please check:
   https://github.com/espressif/esp-idf/tree/master/examples
   This example code is in the Public Domain (or CC0 licensed, at your option.)
   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/
#include "encoder.h"
/* Decode what PCNT's unit originated an interrupt
 * and pass this information together with the event type
 * the main program using a queue.
 */

Encoder *Left;

void setup() {
  Serial.begin(115200);
  /* Initialize PCNT event queue and PCNT functions */
  Left = new Encoder(PCNT_UNIT_0, 16, 17, Encoding::X4, 1023);
  

}

int16_t count = 0;
//pcnt_evt_t evt;
//portBASE_TYPE res;  

void loop() {       
  /* Wait for the event information passed from PCNT's interrupt handler.
   * Once received, decode the event type and print it on the serial monitor.
   */
  if(!(millis() % 100)){
    Serial.println(Left->ReadAndReset());
    delay(1);
  }

}
