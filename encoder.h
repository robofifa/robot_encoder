#ifndef ENCODER_H
#define ENCODER_H

#include "freertos/FreeRTOS.h"
#include "freertos/portmacro.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "driver/periph_ctrl.h"
#include "driver/gpio.h"
#include "driver/pcnt.h"
#include "esp_attr.h"

enum  Encoding{
  X1,
  X2,
  X4,
  Encoding_max,
};

class Encoder{
public:
  Encoder(pcnt_unit_t unit, uint8_t A, uint8_t B, Encoding encoding, uint16_t filter_length);
  Encoder(pcnt_unit_t unit, uint8_t A, uint8_t B, Encoding encoding);
  int Read();
  void Reset();
  int ReadAndReset();
private:
  int16_t flows = 0;
  pcnt_unit_t unit;
  pcnt_isr_handle_t user_isr_handle = NULL; //user's ISR service handle

};


#endif
