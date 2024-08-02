#include "gps.h"
#include "em_device.h"

SoftwareSerial gpsSerial(PIN_GPS_RX, PIN_GPS_TX); // RX, TX
TinyGPSPlus gps;

float ax, ay, az;
float velocityX = 0, velocityY = 0, velocityZ = 0;
float positionX = 0, positionY = 0, positionZ = 0;
unsigned long lastUpdate = 0;

void setup_gps()
{
    Serial.println(TinyGPSPlus::libraryVersion());
    gpsSerial.begin(9600);
    Serial.println(F("INIT GPS 9600 OK"));
}

void read_gps()
{
    Serial.print("read_gps \t");
    // gps 卫星数量少于3个，改变状态
    if (get_gps_data()->satellites < 3)
    {
        if (get_device_state()->gps_state == STATUS_NORMAL)
        {
            get_device_state()->gps_state = STATUS_LACK;
            Serial.println("GPS LACK");
        }
        if (0)
        {
            // 使用IMU数据推算位置
            unsigned long currentTime = millis();
            float dt = (currentTime - lastUpdate) / 1000.0;
            lastUpdate = currentTime;

            // 更新速度和位置
            velocityX += ax * dt;
            velocityY += ay * dt;
            velocityZ += az * dt;

            positionX += velocityX * dt;
            positionY += velocityY * dt;
            positionZ += velocityZ * dt;
            // 更新GPS数据结构
            get_gps_data()->lat += positionY * dt / 111320.0; // 假设在赤道上，1度大约是111.32 km
            get_gps_data()->lng += positionX * dt / (111320.0 * cos(get_gps_data()->lat * PI / 180.0));
            get_gps_data()->altitude += positionZ * dt;
            get_gps_data()->speed = sqrt(velocityX * velocityX + velocityY * velocityY + velocityZ * velocityZ);
            get_gps_data()->direction = atan2(velocityY, velocityX) * 180.0 / PI;
            Serial.print("IMU Cal OK\t");
        }
    }
    else
    {
        if (get_device_state()->gps_state == STATUS_LACK)
            get_device_state()->gps_state = STATUS_NORMAL;
    }

    Serial.printf("Lat: %f\t", get_gps_data()->lat);
    Serial.printf("Lng: %f\t", get_gps_data()->lng);
    Serial.printf("Alt: %f\t", get_gps_data()->altitude);
    Serial.printf("Spd: %f\t", get_gps_data()->speed);
    Serial.printf("Dir: %f\t", get_gps_data()->direction);
    Serial.printf("Sat: %d\t", get_gps_data()->satellites);
    // 时间格式 YYYYMMDD HHMMSS
    Serial.printf("%04d%02d%02d %02d%02d%02d",
                  get_gps_data()->year, get_gps_data()->month, get_gps_data()->day, get_gps_data()->hour, get_gps_data()->minute, get_gps_data()->second);

    Serial.println();
}

// 必须无延时的任务
void loop_gps()
{
    while (gpsSerial.available() > 0)
    {
        // Serial.print((char)gpsSerial.read());

        gps.encode(gpsSerial.read());
        if (gps.location.isValid())
        {
            get_gps_data()->lat = gps.location.lat();
            get_gps_data()->lng = gps.location.lng();
        };

        if (gps.altitude.isValid())
            get_gps_data()->altitude = gps.altitude.meters();

        if (gps.speed.isValid())
            get_gps_data()->speed = gps.speed.kmph();

        if (gps.course.isValid())
            get_gps_data()->direction = gps.course.deg();

        if (gps.satellites.isValid())
            get_gps_data()->satellites = gps.satellites.value();

        if (gps.hdop.isValid())
            get_gps_data()->centisecond = gps.hdop.value();

        if (gps.date.isValid())
        {
            get_gps_data()->year = gps.date.year();
            get_gps_data()->month = gps.date.month();
            get_gps_data()->day = gps.date.day();
        };

        if (gps.time.isValid())
        {
            get_gps_data()->hour = gps.time.hour();
            get_gps_data()->minute = gps.time.minute();
            get_gps_data()->second = gps.time.second();
            get_gps_data()->centisecond = gps.time.centisecond();
        };
    }
}