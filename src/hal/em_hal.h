#ifndef _EM_HAL_H_
#define _EM_HAL_H_

#include "em_config.h"
#include "em_device.h"
#include "gps.h"

typedef enum
{
    BEEP_CONNECT = 0,
    BEEP_WARN,
    BEEP_PRINTER_START,
} beep_type_e;

void read_all_hal();

#endif