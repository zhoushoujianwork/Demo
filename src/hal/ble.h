#ifndef _BLE_H_
#define _BLE_H_

#include <BLEDevice.h>
#include <BLE2902.h>

#include "hal/led.h"
#include "em_config.h"
#include "em_hal.h"
#include "hal/gps.h"
#include "hal/imu.h"

void setup_ble();
bool get_ble_connect();
void ble_report();
#endif