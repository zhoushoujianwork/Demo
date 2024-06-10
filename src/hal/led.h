#ifndef _LED_H_
#define _LED_H_
#include "em_config.h"
#include "hal/em_hal.h"
typedef enum
{
    LED_CONNECT = 0,
    LED_DISCONNECT,
    LED_WARN,
    LED_ERROR,
    LED_BLINK,
} led_type_e;

void setup_led();
void loop_led();
void run_led(led_type_e type);

#endif