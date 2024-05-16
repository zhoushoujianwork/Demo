#ifndef _EM_DEVICE_H_
#define _EM_DEVICE_H_

#include "em_config.h"

typedef enum
{
    PRINTER_STATUS_INIT = 0,
    PRINTER_STATUS_START,
    PRINTER_STATUS_WORKING,
    PRINTER_STATUS_FINISH,
} printer_state_e;

typedef enum
{
    GPS_STATUS_NORMAL = 0,
    GPS_STATUS_LACK,
} gps_state_e;

typedef struct
{
    uint16_t year;
    uint8_t month;
    uint8_t day;
    uint8_t hour;
    uint8_t minute;
    uint8_t second;
    uint8_t centisecond; // 百分之一秒
    double lat;          // 纬度
    double lng;          // 经度
    double altitude;     // m 高度
    double speed;        // km/h
    double direction;    // 度 0~360
    uint8_t satellites;  // 卫星数
} gps_data_s;

typedef struct
{
    uint8_t battery;
    uint8_t temperature;
    gps_state_e gps_state;
    printer_state_e printer_state;
    gps_data_s gps_data;
    bool read_ble_finish;
} device_state_t;

void init_device_state();

device_state_t *get_device_state();

void set_read_ble_finish(bool finish);

void set_device_paper_status(gps_state_e status);

#endif