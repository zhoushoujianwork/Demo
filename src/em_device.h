#ifndef _EM_DEVICE_H_
#define _EM_DEVICE_H_

#include "em_config.h"

typedef enum
{
    STATUS_NORMAL = 0,
    STATUS_LACK,
} state_e;

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
} gps_data_t;

typedef struct
{
    float ax;
    float ay;
    float az;
    float gx;
    float gy;
    float gz;
    float roll;
    float pitch;
    float yaw;
    float temperature;
} imu_data_t;

typedef struct
{
    uint8_t battery;
    uint8_t temperature;
    state_e gps_state;
    state_e imu_state;
} device_state_t;

void init_device_state();

device_state_t *get_device_state();

gps_data_t *get_gps_data();

imu_data_t *get_imu_data();

void set_device_gps_status(state_e status);

void set_device_imu_status(state_e status);

#endif