#include "gps.h"
#include "em_device.h"

SoftwareSerial gpsSerial(PIN_GPS_RX, PIN_GPS_TX); // RX, TX
TinyGPSPlus gps;

void setup_gps()
{
    Serial.println(TinyGPSPlus::libraryVersion());
    gpsSerial.begin(9600);
    Serial.println(F("INIT GPS 9600 OK"));
}

void read_gps()
{
    Serial.print("read_gps \t");

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

    // gps 卫星数量少于3个，改变状态
    if (get_gps_data()->satellites < 3)
    {
        if (get_device_state()->gps_state == STATUS_NORMAL)
        {
            get_device_state()->gps_state = STATUS_LACK;
            Serial.println("GPS LACK");
        }
    }
    else
    {
        if (get_device_state()->gps_state == STATUS_LACK)
            get_device_state()->gps_state = STATUS_NORMAL;

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
}

// 必须无延时的任务
void loop_gps()
{
    while (gpsSerial.available() > 0)
    {
        // Serial.print((char)gpsSerial.read());
        gps.encode(gpsSerial.read());
    }
}