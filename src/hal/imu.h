#ifndef _imu_H_
#define _imu_H_
#include "em_config.h"
#include "em_device.h"

void setup_imu();
void load_imu();
void load_imu_kalman();
void read_imu();
#endif