// Basic demo for accelerometer readings from Adafruit MPU6050
#include "imu.h"
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>
#include "em_device.h"

Adafruit_MPU6050 mpu;
sensors_event_t a, g, temp;

void setup_imu(void)
{

    // Try to initialize!
    if (!mpu.begin())
    {
        Serial.println("Failed to find MPU6050 chip");
        while (1)
        {
            delay(10);
        }
    }
    Serial.println("MPU6050 Found!");

    mpu.setAccelerometerRange(MPU6050_RANGE_8_G);
    Serial.print("Accelerometer range set to: ");
    switch (mpu.getAccelerometerRange())
    {
    case MPU6050_RANGE_2_G:
        Serial.println("+-2G");
        break;
    case MPU6050_RANGE_4_G:
        Serial.println("+-4G");
        break;
    case MPU6050_RANGE_8_G:
        Serial.println("+-8G");
        break;
    case MPU6050_RANGE_16_G:
        Serial.println("+-16G");
        break;
    }
    mpu.setGyroRange(MPU6050_RANGE_500_DEG);
    Serial.print("imu range set to: ");
    switch (mpu.getGyroRange())
    {
    case MPU6050_RANGE_250_DEG:
        Serial.println("+- 250 deg/s");
        break;
    case MPU6050_RANGE_500_DEG:
        Serial.println("+- 500 deg/s");
        break;
    case MPU6050_RANGE_1000_DEG:
        Serial.println("+- 1000 deg/s");
        break;
    case MPU6050_RANGE_2000_DEG:
        Serial.println("+- 2000 deg/s");
        break;
    }

    mpu.setFilterBandwidth(MPU6050_BAND_21_HZ);
    Serial.print("Filter bandwidth set to: ");
    switch (mpu.getFilterBandwidth())
    {
    case MPU6050_BAND_260_HZ:
        Serial.println("260 Hz");
        break;
    case MPU6050_BAND_184_HZ:
        Serial.println("184 Hz");
        break;
    case MPU6050_BAND_94_HZ:
        Serial.println("94 Hz");
        break;
    case MPU6050_BAND_44_HZ:
        Serial.println("44 Hz");
        break;
    case MPU6050_BAND_21_HZ:
        Serial.println("21 Hz");
        break;
    case MPU6050_BAND_10_HZ:
        Serial.println("10 Hz");
        break;
    case MPU6050_BAND_5_HZ:
        Serial.println("5 Hz");
        break;
    }

    Serial.println("");
    delay(100);
}

// // 定义平均滤波数组大小
// #define FILTER_SIZE 4 // 缓冲区大小 和刷新速度有关 越快越小，否则会有延迟
// float bufferRoll[FILTER_SIZE];
// int roll_index = 0; // 缓冲区索引
// float bufferPitch[FILTER_SIZE];
// int pitch_index = 0;
// float bufferYaw[FILTER_SIZE];
// int yaw_index = 0;

// /*
//     一维滤波
//     param: 数组bufferRoll 缓冲区,newData 新数据
// */
// void addData(float newData, float *bufferRoll, int &roll_index)
// {
//     bufferRoll[roll_index] = newData;
//     // 循环更新缓冲区索引
//     roll_index = (roll_index + 1) % FILTER_SIZE;
// }

// // 计算平均值
// float getFilteredData(float *buffer)
// {
//     float average = 0;
//     for (int i = 0; i < FILTER_SIZE; i++)
//     {
//         average += buffer[i];
//     }
//     return average / FILTER_SIZE;
// }

void read_imu()
{
    mpu.getEvent(&a, &g, &temp);

    /* Print out the values */
    get_imu_data()->ax = a.acceleration.x;
    get_imu_data()->ay = a.acceleration.y;
    get_imu_data()->az = a.acceleration.z;
    get_imu_data()->gx = g.gyro.x;
    get_imu_data()->gy = g.gyro.y;
    get_imu_data()->gz = g.gyro.z;
    get_imu_data()->roll = a.gyro.roll;
    get_imu_data()->pitch = a.gyro.pitch;
    get_imu_data()->yaw = a.gyro.heading;

    Serial.print("Roll: ");
    Serial.print(get_imu_data()->roll);
    Serial.print(", Pitch: ");
    Serial.print(get_imu_data()->pitch);
    Serial.print(", Yaw: ");
    Serial.print(get_imu_data()->yaw);

    // imu 温度
    Serial.print(", Temperature: ");
    Serial.print(temp.temperature);
    get_imu_data()->temperature = temp.temperature;

    Serial.println("");
}
