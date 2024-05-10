#ifndef _EM_CONFIG_H_
#define _EM_CONFIG_H_

#include <Arduino.h>
#define LedPin 18 // LED引脚 GPIO18

#define BtnPin 5           // 按键引脚 GPIO5
#define ShortPressTime 500 // 短按时间 单位毫秒

#define PIN_BAT_ADC 34 // 电池电压检测引脚 GPIO36 需要支持ADC的引脚
#define PIN_BAT_EN 4   // ADC使能引脚 GPIO4
#define PIN_BAT_BIT 12 // ADC采样次数 0-4095

#endif