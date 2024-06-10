#include "em_hal.h"
#include "hal/bat.h"
#include "hal/imu.h"

void read_all_hal()
{
    read_battery();

    // read_temperature();
    // read_paper_statue();
}