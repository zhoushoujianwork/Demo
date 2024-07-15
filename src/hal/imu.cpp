#include "imu.h"
#include <Wire.h>
#include <SPI.h>
#include "SensorQMI8658.hpp"

#define USE_WIRE

#ifndef SENSOR_SDA
#define SENSOR_SDA 21
#endif

#ifndef SENSOR_SCL
#define SENSOR_SCL 22
#endif

#ifndef SENSOR_IRQ
#define SENSOR_IRQ -1
#endif

#define IMU_CS 5

SensorQMI8658 qmi;

IMUdata acc;
IMUdata gyr;

void setup_imu()
{
#ifdef USE_WIRE
    // Using WIRE !!
    if (!qmi.begin(Wire, QMI8658_L_SLAVE_ADDRESS, SENSOR_SDA, SENSOR_SCL))
    {
        Serial.println("Failed to find QMI8658 - check your wiring!");
        while (1)
        {
            delay(1000);
        }
    }
#else
    if (!qmi.begin(IMU_CS))
    {
        Serial.println("Failed to find QMI8658 - check your wiring!");
        while (1)
        {
            delay(1000);
        }
    }
#endif

    /* Get chip id*/
    Serial.print("Device ID:");
    Serial.println(qmi.getChipID(), HEX);

    qmi.configAccelerometer(
        /*
         * ACC_RANGE_2G
         * ACC_RANGE_4G
         * ACC_RANGE_8G
         * ACC_RANGE_16G
         * */
        SensorQMI8658::ACC_RANGE_4G,
        /*
         * ACC_ODR_1000H
         * ACC_ODR_500Hz
         * ACC_ODR_250Hz
         * ACC_ODR_125Hz
         * ACC_ODR_62_5Hz
         * ACC_ODR_31_25Hz
         * ACC_ODR_LOWPOWER_128Hz
         * ACC_ODR_LOWPOWER_21Hz
         * ACC_ODR_LOWPOWER_11Hz
         * ACC_ODR_LOWPOWER_3H
         * */
        SensorQMI8658::ACC_ODR_1000Hz,
        /*
         *  LPF_MODE_0     //2.66% of ODR
         *  LPF_MODE_1     //3.63% of ODR
         *  LPF_MODE_2     //5.39% of ODR
         *  LPF_MODE_3     //13.37% of ODR
         * */
        SensorQMI8658::LPF_MODE_0,
        // selfTest enable
        true);

    qmi.configGyroscope(
        /*
         * GYR_RANGE_16DPS
         * GYR_RANGE_32DPS
         * GYR_RANGE_64DPS
         * GYR_RANGE_128DPS
         * GYR_RANGE_256DPS
         * GYR_RANGE_512DPS
         * GYR_RANGE_1024DPS
         * */
        SensorQMI8658::GYR_RANGE_64DPS,
        /*
         * GYR_ODR_7174_4Hz
         * GYR_ODR_3587_2Hz
         * GYR_ODR_1793_6Hz
         * GYR_ODR_896_8Hz
         * GYR_ODR_448_4Hz
         * GYR_ODR_224_2Hz
         * GYR_ODR_112_1Hz
         * GYR_ODR_56_05Hz
         * GYR_ODR_28_025H
         * */
        SensorQMI8658::GYR_ODR_896_8Hz,
        /*
         *  LPF_MODE_0     //2.66% of ODR
         *  LPF_MODE_1     //3.63% of ODR
         *  LPF_MODE_2     //5.39% of ODR
         *  LPF_MODE_3     //13.37% of ODR
         * */
        SensorQMI8658::LPF_MODE_3,
        // selfTest enable
        true);

    // In 6DOF mode (accelerometer and gyroscope are both enabled),
    // the output data rate is derived from the nature frequency of gyroscope
    qmi.enableGyroscope();
    qmi.enableAccelerometer();

    // Print register configuration information
    qmi.dumpCtrlRegister();

    Serial.println("Read data now...");
}

void read_imu()
{
    Serial.print("Roll: ");
    Serial.print(get_imu_data()->roll);
    Serial.print(", Pitch: ");
    Serial.print(get_imu_data()->pitch);
    Serial.print(", Yaw: ");
    Serial.print(get_imu_data()->yaw);

    // imu 温度
    Serial.print(", Temperature: ");
    Serial.print(get_imu_data()->temperature);
    Serial.println("");
}

// 互补滤波参数
const float alpha = 0.98;
float angleX = 0.0;
float angleY = 0.0;
float angleZ = 0.0;
long previousTime;

void load_imu()
{

    long currentTime = millis();
    float elapsedTime = (currentTime - previousTime) / 1000.0;
    previousTime = currentTime;

    if (qmi.getDataReady())
    {

        if (qmi.getAccelerometer(acc.x, acc.y, acc.z))
        {
            get_imu_data()->ax = acc.x;
            get_imu_data()->ay = acc.y;
            get_imu_data()->az = acc.z;
        }

        if (qmi.getGyroscope(gyr.x, gyr.y, gyr.z))
        {
            get_imu_data()->gx = gyr.x;
            get_imu_data()->gy = gyr.y;
            get_imu_data()->gz = gyr.z;
        }
    }
    // 互补滤波计算
    angleX = alpha * (angleX + gyr.x * elapsedTime) + (1 - alpha) * acc.x;
    angleY = alpha * (angleY + gyr.y * elapsedTime) + (1 - alpha) * acc.y;
    angleZ = alpha * (angleZ + gyr.z * elapsedTime) + (1 - alpha) * acc.z;

    get_imu_data()->roll = angleX;
    get_imu_data()->pitch = angleY;
    get_imu_data()->yaw = angleZ;

    get_imu_data()->temperature = qmi.getTemperature_C();
    read_imu();
}
