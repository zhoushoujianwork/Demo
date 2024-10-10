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

unsigned long lastTime;
float vx = 0, vy = 0, vz = 0;

// 通过 IMU 数据实现速度计算
void speed_cal()
{
    // 当前时间
    unsigned long currentTime = millis();
    float deltaTime = (currentTime - lastTime) / 1000.0; // 将时间差转换为秒
    lastTime = currentTime;

    // 简单积分计算速度
    vx += get_imu_data()->ax * deltaTime;
    vy += get_imu_data()->ay * deltaTime;
    vz += get_imu_data()->az * deltaTime;

    // 打印速度值
    Serial.print("Vx:");
    Serial.print(vx);
    Serial.print("Vy:");
    Serial.print(vy);
    Serial.print("Vz:");
    Serial.println(vz);
}

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
    Serial.print("ax:");
    Serial.print(get_imu_data()->ax);
    Serial.print("\t");
    Serial.print("ay:");
    Serial.print(get_imu_data()->ay);
    Serial.print("\t");
    Serial.print("az:");
    Serial.print(get_imu_data()->az);
    Serial.print("\t");
    Serial.print("Roll:");
    Serial.print(get_imu_data()->roll);
    Serial.print("\t");
    Serial.print("Pitch:");
    Serial.print(get_imu_data()->pitch);
    Serial.print("\t");
    Serial.print("Yaw:");
    Serial.print(get_imu_data()->yaw);
    Serial.print("\t");
    Serial.print("Temperature:");
    Serial.print(get_imu_data()->temperature);
    Serial.println("");
}

// 互补滤波实现
#define ALPHA 0.98 // 互补滤波的系数，范围在0到1之间
float dt = 0.01;   // 时间间隔，单位是秒（假设采样率为100Hz）
float roll_gyro = 0.0;
float pitch_gyro = 0.0;

void load_imu()
{
    // speed_cal();

    if (qmi.getDataReady())
    {
        if (qmi.getAccelerometer(acc.x, acc.y, acc.z))
        {
            get_imu_data()->ax = acc.x;
            get_imu_data()->ay = acc.y;
            get_imu_data()->az = acc.z;
            // Serial.printf("acc:%f,%f,%f\t"acc.x, acc.y, acc.z);
        }

        if (qmi.getGyroscope(gyr.x, gyr.y, gyr.z))
        {
            get_imu_data()->gx = gyr.x;
            get_imu_data()->gy = gyr.y;
            get_imu_data()->gz = gyr.z;
            // Serial.printf("gyr:%f,%f,%f\t"gyr.x, gyr.y, gyr.z);
        }

        // 计算加速度计的Roll和Pitch
        float roll_acc = atan2(acc.y, acc.z) * 180 / M_PI;
        float pitch_acc = atan2(-acc.x, sqrt(acc.y * acc.y + acc.z * acc.z)) * 180 / M_PI;

        // 角速度转换为角度增量
        float roll_gyro_delta = gyr.x * dt;
        float pitch_gyro_delta = gyr.y * dt;

        // 计算通过积分得到的Roll和Pitch（仅使用陀螺仪数据）
        roll_gyro += roll_gyro_delta;
        pitch_gyro += pitch_gyro_delta;

        // 使用互补滤波器结合加速度计和陀螺仪的数据
        get_imu_data()->roll = ALPHA * (roll_gyro) + (1.0 - ALPHA) * roll_acc;
        get_imu_data()->pitch = ALPHA * (pitch_gyro) + (1.0 - ALPHA) * pitch_acc;

        // QMI事件的温度数据
        get_imu_data()->temperature = qmi.getTemperature_C();
        // read_imu();
    }
}

struct KalmanFilter
{
    float q; // 过程噪声协方差
    float r; // 测量噪声协方差
    float x; // 估计值
    float p; // 估计误差协方差
    float k; // 卡尔曼增益
};

// 定义加速度计三个轴的卡尔曼滤波器实例
KalmanFilter kf_ax = {0.001, 0.1, 0, 1, 0};
KalmanFilter kf_ay = {0.001, 0.1, 0, 1, 0};
KalmanFilter kf_az = {0.001, 0.1, 0, 1, 0};

// 定义姿态估计的卡尔曼滤波器实例
struct AttitudeKalmanFilter
{
    float qAngle;
    float qBias;
    float rMeasure;
    float angle;
    float bias;
    float rate;
    float P[2][2];
};

AttitudeKalmanFilter kalmanRoll = {0.001, 0.003, 0.03, 0, 0, 0, {{1, 0}, {0, 1}}};
AttitudeKalmanFilter kalmanPitch = {0.001, 0.003, 0.03, 0, 0, 0, {{1, 0}, {0, 1}}};

float kalmanUpdate(KalmanFilter &kf, float measurement)
{
    // 预测
    kf.p += kf.q;

    // 更新
    kf.k = kf.p / (kf.p + kf.r);
    kf.x += kf.k * (measurement - kf.x);
    kf.p *= (1 - kf.k);

    return kf.x;
}

float kalmanUpdate(AttitudeKalmanFilter &kf, float newAngle, float newRate, float dt)
{
    // Predict
    kf.rate = newRate - kf.bias;
    kf.angle += dt * kf.rate;

    kf.P[0][0] += dt * (dt * kf.P[1][1] - kf.P[0][1] - kf.P[1][0] + kf.qAngle);
    kf.P[0][1] -= dt * kf.P[1][1];
    kf.P[1][0] -= dt * kf.P[1][1];
    kf.P[1][1] += kf.qBias * dt;

    // Update
    float S = kf.P[0][0] + kf.rMeasure;
    float K[2];
    K[0] = kf.P[0][0] / S;
    K[1] = kf.P[1][0] / S;

    float y = newAngle - kf.angle;
    kf.angle += K[0] * y;
    kf.bias += K[1] * y;

    float P00_temp = kf.P[0][0];
    float P01_temp = kf.P[0][1];

    kf.P[0][0] -= K[0] * P00_temp;
    kf.P[0][1] -= K[0] * P01_temp;
    kf.P[1][0] -= K[1] * P00_temp;
    kf.P[1][1] -= K[1] * P01_temp;

    return kf.angle;
}
// 卡尔曼滤波实现
void load_imu_kalman()
{
    if (qmi.getDataReady())
    {
        if (qmi.getAccelerometer(acc.x, acc.y, acc.z))
        {
            // 使用卡尔曼滤波器对加速度计测量值进行平滑处理
            get_imu_data()->ax = kalmanUpdate(kf_ax, acc.x);
            get_imu_data()->ay = kalmanUpdate(kf_ay, acc.y);
            get_imu_data()->az = kalmanUpdate(kf_az, acc.z);
        }

        if (qmi.getGyroscope(gyr.x, gyr.y, gyr.z))
        {
            get_imu_data()->gx = gyr.x;
            get_imu_data()->gy = gyr.y;
            get_imu_data()->gz = gyr.z;
        }

        // float dt = 0.01; // 假设采样率为100Hz
        float dt = 0.1; // 假设采样率为10Hz

        // 计算加速度计的Roll和Pitch
        float roll_acc = atan2(acc.y, acc.z) * 180 / M_PI;
        float pitch_acc = atan2(-acc.x, sqrt(acc.y * acc.y + acc.z * acc.z)) * 180 / M_PI;

        // 使用卡尔曼滤波
        get_imu_data()->roll = kalmanUpdate(kalmanRoll, roll_acc, gyr.x, dt);
        get_imu_data()->pitch = kalmanUpdate(kalmanPitch, pitch_acc, gyr.y, dt);

        get_imu_data()->temperature = qmi.getTemperature_C();
    }
}