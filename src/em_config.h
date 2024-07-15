#ifndef _EM_CONFIG_H_
#define _EM_CONFIG_H_

#include <Arduino.h>
#define LedPin 18 // LED引脚

#define BtnPin 5           // 按键引脚
#define ShortPressTime 500 // 短按时间 单位毫秒

#define PIN_BAT_ADC 34 // 电池电压检测引脚 GPIO36 需要支持ADC的引脚
// #define PIN_BAT_BIT 12 // ADC采样次数 0-4095

#define ENABLE_GPS 0
#define ENABLE_IMU 1
#define ENABLE_BLE 1

// 定义IMU模块的引脚
#define PIN_IMU_SDA 21 // IMU模块接入 I2C SDA
#define PIN_IMU_SCL 22 // IMU模块 I2C SCL

// 定义GPS模块的引脚
#define PIN_GPS_RX 23 // GPS模块RX
#define PIN_GPS_TX 19 // GPS模块TX

// BLE
#define BLE_NAME "ESP32-MOTO"
#define SERVICE_UUID "4fafc201-1fb5-459e-8fcc-c5c9c331914b"            // 自定义打印UUID
#define CHARACTERISTIC_UUID "beb5483e-36e1-4688-b7f5-ea07361b2600"     // 自定义打印UUID
#define CHARACTERISTIC_UUID_GPS "beb5483e-36e1-4688-b7f5-ea07361b2601" // 自定义打印UUID
#define CHARACTERISTIC_UUID_IMU "beb5483e-36e1-4688-b7f5-ea07361b2602" // 自定义打印UUID
#define BROADCAST_INTERVAL 10000                                       // 广播间隔时间 单位毫秒

#endif
