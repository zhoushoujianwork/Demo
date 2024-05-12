#include "em_task.h"
#include "hal/led.h"
#include "hal/btn.h"
#include "hal/bat.h"
#include "hal/gps.h"

void loop_led(void *pvParameters)
{
    for (;;) // A Task shall never return or exit.
    {
        loop_led();
        vTaskDelay(100);
    }
}

void loop_btn(void *pvParameters)
{
    for (;;) // A Task shall never return or exit.
    {
        loop_btn();
        vTaskDelay(10);
    }
}

void loop_bat(void *pvParameters)
{
    for (;;) // A Task shall never return or exit.
    {
        loop_bat();
        vTaskDelay(1000);
    }
}

void loop_gps(void *pvParameters)
{
    for (;;) // A Task shall never return or exit.
    {
        displayInfo();
        vTaskDelay(1000);
    }
}

void init_task()
{
    Serial.begin(115200);
    Serial.print("init_task\n");
    Serial.print("--mini system--\n");

    setup_led();
    setup_bat();
    setup_btn();
    setup_gps();
    xTaskCreate(
        loop_led,   // 任务函数
        "LED_TASK", // 任务名
        1024 * 10,  // 任务栈
        NULL,       // 任务参数
        1,          // 任务优先级, with 3 (configMAX_PRIORITIES - 1) 是最高的，0是最低的.
        NULL        // 任务句柄
    );
    xTaskCreate(
        loop_btn,   // 任务函数
        "BTN_TASK", // 任务名
        1024 * 10,  // 任务栈
        NULL,       // 任务参数
        1,          // 任务优先级, with 3 (configMAX_PRIORITIES - 1) 是最高的，0是最低的.
        NULL        // 任务句柄
    );
    xTaskCreate(
        loop_bat,   // 任务函数
        "BAT_TASK", // 任务名
        1024 * 10,  // 任务栈
        NULL,       // 任务参数
        1,          // 任务优先级, with 3 (configMAX_PRIORITIES - 1) 是最高的，0是最低的.
        NULL        // 任务句柄
    );
    xTaskCreate(
        loop_gps,   // 任务函数
        "GPS_TASK", // 任务名
        1024 * 10,  // 任务栈
        NULL,       // 任务参数
        1,          // 任务优先级, with 3 (configMAX_PRIORITIES - 1) 是最高的，0是最低的.
        NULL        // 任务句柄
    );
}

void loop_task()
{
    loop_gps();
}