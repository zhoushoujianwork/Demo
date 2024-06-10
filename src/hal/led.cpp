
#include "led.h"

/*
 * LED相关
 Connect: BLE 连接后常亮；
 Disconnect: BLE 连接断开后关闭
 Warn: 一次闪烁
 Blink: 两次闪烁
 Error: 三次闪烁
 */
void run_led(led_type_e type)
{
  // debug
  Serial.printf("run_led %d\n", type);

  switch (type)
  {
  case LED_BLINK:
    // 2次闪烁 LedPin
    Serial.println("LED_BLINK");
    digitalWrite(LedPin, HIGH);
    delay(500);
    digitalWrite(LedPin, LOW);
    delay(500);
    digitalWrite(LedPin, HIGH);
    delay(500);
    digitalWrite(LedPin, LOW);
    delay(500);
    digitalWrite(LedPin, HIGH);
    break;
  case LED_WARN:
    // 一次闪烁 LedPin
    Serial.println("LED_WARN");
    digitalWrite(LedPin, HIGH);
    delay(500);
    digitalWrite(LedPin, LOW);
    delay(500);
    digitalWrite(LedPin, HIGH);
    break;
  case LED_CONNECT:
    Serial.println("LED_CONNECT");
    digitalWrite(LedPin, LOW);
    break;
  case LED_DISCONNECT:
    Serial.println("LED_DISCONNECT");
    digitalWrite(LedPin, HIGH);
    break;
  case LED_ERROR:
    // 3次闪烁 LedPin
    Serial.println("LED_ERROR");
    digitalWrite(LedPin, HIGH);
    delay(500);
    digitalWrite(LedPin, LOW);
    delay(500);
    digitalWrite(LedPin, HIGH);
    delay(500);
    digitalWrite(LedPin, LOW);
    delay(500);
    digitalWrite(LedPin, HIGH);
    delay(500);
    digitalWrite(LedPin, LOW);
    delay(500);
    digitalWrite(LedPin, HIGH);
    break;
  }
}

void setup_led()
{
  pinMode(LedPin, OUTPUT);
  delay(1000);
  run_led(LED_WARN);
  delay(1000);
  run_led(LED_BLINK);
  delay(1000);
  run_led(LED_ERROR);

  Serial.println("LED Ready");
}

void loop_led()
{
}
