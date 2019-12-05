#include "Encoder.h"


#define PCNT_H_LIM_VAL      INT16_MAX/100
#define PCNT_L_LIM_VAL     -INT16_MAX/100
#define PCNT_THRESH1_VAL    PCNT_H_LIM_VAL/2
#define PCNT_THRESH0_VAL    PCNT_L_LIM_VAL/2
#define PCNT_FILTER_LENGTH  1023

static void IRAM_ATTR pcnt_example_intr_handler(void *arg)
{
  uint32_t intr_status = PCNT.int_st.val;
  int i;

  for (i = 0; i < PCNT_UNIT_MAX; i++) {
    if (intr_status & (BIT(i))) {
      if (PCNT.status_unit[i].val & 16) {
        *static_cast<uint16_t*>(arg) -= 1;
      }
      if (PCNT.status_unit[i].val & 32) {
        *static_cast<uint16_t*>(arg) += 1;
      }
      PCNT.int_clr.val = BIT(i);
    }
  }
}

Encoder::Encoder(pcnt_unit_t unit, uint8_t A, uint8_t B, Encoding encoding, uint16_t filter_length) : unit(unit){


    /* Prepare configuration for the PCNT unit */
    pcnt_config_t pcnt_config_CH0 = {
        // Set PCNT input signal and control GPIOs
        .pulse_gpio_num = A,
        .ctrl_gpio_num = B,
        .lctrl_mode = PCNT_MODE_KEEP, // Reverse counting direction if low
        .hctrl_mode = PCNT_MODE_REVERSE,    // Keep the primary counter mode if
        .pos_mode = PCNT_COUNT_INC,   // Count up on the positive edge
        .neg_mode = PCNT_COUNT_DEC,   // Keep the counter value on the negative edge
        .counter_h_lim = PCNT_H_LIM_VAL,
        .counter_l_lim = PCNT_L_LIM_VAL,
        .unit = unit,
        .channel = PCNT_CHANNEL_0,
    };
    /* Initialize PCNT unit */
    pcnt_unit_config(&pcnt_config_CH0);

    pcnt_config_t pcnt_config_CH1 = {
        // Set PCNT input signal and control GPIOs
        .pulse_gpio_num = B,
        .ctrl_gpio_num = A,
        .lctrl_mode = PCNT_MODE_REVERSE, // Reverse counting direction if low
        .hctrl_mode = PCNT_MODE_KEEP,    // Keep the primary counter mode if
        .pos_mode = PCNT_COUNT_INC,   // Count up on the positive edge
        .neg_mode = PCNT_COUNT_DEC,   // Keep the counter value on the negative edge
        .counter_h_lim = PCNT_H_LIM_VAL,
        .counter_l_lim = PCNT_L_LIM_VAL,
        .unit = unit,
        .channel = PCNT_CHANNEL_1,
    };
    /* Initialize PCNT unit */
    pcnt_unit_config(&pcnt_config_CH1);

    /* Configure and enable the input filter */
    pcnt_set_filter_value(unit, filter_length);
    pcnt_filter_enable(unit);

    /* Set threshold 0 and 1 values and enable events to watch */
//    pcnt_set_event_value(unit, PCNT_EVT_THRES_1, PCNT_THRESH1_VAL);
//    pcnt_event_enable(unit, PCNT_EVT_THRES_1);
//    pcnt_set_event_value(unit, PCNT_EVT_THRES_0, PCNT_THRESH0_VAL);
//    pcnt_event_enable(unit, PCNT_EVT_THRES_0);
    /* Enable events on zero, maximum and minimum limit values */
//    pcnt_event_enable(unit, PCNT_EVT_ZERO);
    pcnt_event_enable(unit, PCNT_EVT_H_LIM);
    pcnt_event_enable(unit, PCNT_EVT_L_LIM);

    /* Initialize PCNT's counter */
    pcnt_counter_pause(unit);
    pcnt_counter_clear(unit);

    /* Register ISR handler and enable interrupts for PCNT unit */
    pcnt_isr_register(pcnt_example_intr_handler, &this->flows, 0, &this->user_isr_handle);
    pcnt_intr_enable(unit);

    /* Everything is set up, now go to counting */
    pcnt_counter_resume(unit);
}

Encoder::Encoder(pcnt_unit_t unit, uint8_t A, uint8_t B, Encoding encoding) : Encoder(unit, A, B, encoding, 0){

}

int Encoder::Read(){
  int16_t count = 0;
  pcnt_get_counter_value(this->unit, &count);

  return count + flows * PCNT_H_LIM_VAL;
}

void Encoder::Reset(){
    pcnt_counter_pause(this->unit);
    pcnt_counter_clear(this->unit);
    flows = 0;
    pcnt_counter_resume(this->unit);
}

int Encoder::ReadAndReset(){
  int count = Read();
  Reset();
  return count;
}
