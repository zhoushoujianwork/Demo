#ifndef _GPS_H_
#define _GPS_H_
#include "em_config.h"
#include <TinyGPSPlus.h>
#include <SoftwareSerial.h>

void setup_gps();
void loop_gps();
void read_gps();
#endif