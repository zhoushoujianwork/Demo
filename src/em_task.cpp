#include "em_task.h"
#include "hal/led.h"
#include "hal/btn.h"
#include "hal/gps.h"
#include "hal/ble.h"
#include "hal/bat.h"
#include "hal/imu.h"
#include "hal/em_timer.h"

void run_report()
{
    if (get_state_timeout())
    {
        clean_state_timeout();
        read_all_hal();

#if ENABLE_BLE
        if (get_ble_connect())
        {
            Serial.print("report device status ： report time up\n");
            ble_report();
            read_gps();
            read_imu();
        }
#endif
    }
}

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

void task_report(void *pvParameters)
{
    for (;;) // A Task shall never return or exit.
    {
        run_report();
        load_imu();
        vTaskDelay(100);
#if ENABLE_IMU
        // read_imu();
#endif
#if ENABLE_GPS
        // read_gps();
#endif
    }
}

void init_task()
{
    Serial.begin(115200);

    // 清理终端输出
    Serial.println("");

    init_device_state();
    init_timer();

    setup_led();
    setup_bat();
    setup_btn();
#if ENABLE_BLE
    setup_ble();
#endif

#if ENABLE_GPS
    setup_gps();
#endif

#if ENABLE_IMU
    setup_imu();
#endif

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
        task_report,  // 任务函数
        "TaskReport", // 任务名
        1024 * 10,    // 任务栈
        NULL,         // 任务参数
        1,            // 任务优先级, with 3 (configMAX_PRIORITIES - 1) 是最高的，0是最低的.
        NULL          // 任务句柄
    );

    Serial.println("init_task ok");
}

void loop_task()
{
#if ENABLE_GPS
    loop_gps();
#endif
}