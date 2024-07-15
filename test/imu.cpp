// Basic demo for accelerometer readings from Adafruit MPU6050
#include "imu.h"
#include <Adafruit_MPU6050.h>
// #include <Adafruit_Sensor.h>
#include "em_device.h"

Adafruit_MPU6050 mpu;

// 互补滤波参数
const float alpha = 0.98;
float angleX = 0.0;
float angleY = 0.0;
float angleZ = 0.0;
long previousTime;

void setup_imu(void)
{

    // Try to initialize!
    if (!mpu.begin())
    {
        Serial.println("Failed to find MPU6050 chip");
        return;
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

void load_imu()
{
    long currentTime = millis();
    float elapsedTime = (currentTime - previousTime) / 1000.0;
    previousTime = currentTime;

    sensors_event_t a, g, temp;
    mpu.getEvent(&a, &g, &temp);

    // 互补滤波计算
    angleX = alpha * (angleX + g.gyro.x * elapsedTime) + (1 - alpha) * a.acceleration.x;
    angleY = alpha * (angleY + g.gyro.y * elapsedTime) + (1 - alpha) * a.acceleration.y;
    angleZ = alpha * (angleZ + g.gyro.z * elapsedTime) + (1 - alpha) * a.acceleration.z;

    get_imu_data()->ax = a.acceleration.x;
    get_imu_data()->ay = a.acceleration.y;
    get_imu_data()->az = a.acceleration.z;
    get_imu_data()->gx = g.gyro.x;
    get_imu_data()->gy = g.gyro.y;
    get_imu_data()->gz = g.gyro.z;
    get_imu_data()->roll = angleX;
    get_imu_data()->pitch = angleY;
    get_imu_data()->yaw = angleZ;
    // get_imu_data()->roll = a.gyro.roll;
    // get_imu_data()->pitch = a.gyro.pitch;
    // get_imu_data()->yaw = a.gyro.heading;

    get_imu_data()->temperature = temp.temperature;
}

void read_imu()
{
    Serial.print("Roll: ");
    Serial.print(get_imu_data()->roll);
    Serial.print(", angleX: ");
    Serial.print(angleX);
    Serial.print(", Pitch: ");
    Serial.print(get_imu_data()->pitch);
    Serial.print(", angleY: ");
    Serial.print(angleY);
    Serial.print(", Yaw: ");
    Serial.print(get_imu_data()->yaw);
    Serial.print(", AngleZ: ");
    Serial.print(angleZ);

    // imu 温度
    Serial.print(", Temperature: ");
    Serial.print(get_imu_data()->temperature);
    Serial.println("");
}