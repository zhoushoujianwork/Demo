#include "gps.h"
#include "em_device.h"

SoftwareSerial gpsSerial(PIN_GPS_RX, PIN_GPS_TX); // RX, TX
TinyGPSPlus gps;

// NMEA0183 配置
// PCAS语句是由芯片供应商定义的NMEA专用语句

// 设置波特率为 115200
const char *UBX_CFG_PRT_115200 =
    "$PCAS01,5*19\r\n";
// 57600
const char *UBX_CFG_PRT_57600 =
    "$PCAS01,4*18\r\n";
// 9600
const char *UBX_CFG_PRT_9600 =
    "$PCAS01,1*1D\r\n";

// 设置 NMEA 更新速率为 1Hz
const char *UBX_CFG_RATE_1HZ =
    "$PCAS02,1000*2E\r\n";
// 设置 NMEA 更新速率为 2Hz
const char *UBX_CFG_RATE_2HZ =
    "$PCAS02,500*1A\r\n";
// 设置 NMEA 更新速率为 5Hz
const char *UBX_CFG_RATE_5HZ =
    "$PCAS02,200*1D\r\n";
// 设置 10Hz $PCAS02,100*1E
const char *UBX_CFG_RATE_10HZ =
    "$PCAS02,100*1E\r\n";

// 设置 $PCAS03,0,0,0,0,1,0,0,003（只选择RMC）
const char *UBX_CFG_PRT_1 =
    "$PCAS03,0,0,0,0,1,0,0,0*03\r\n";
// 设置 $PCAS03,1,0,0,0,1,0,0,002 GGA RMC
const char *UBX_CFG_PRT_2 =
    "$PCAS03,1,0,0,0,1,0,0,0*02\r\n";
const char *UBX_CFG_PRT_3 =
    "$PCAS03,1,1,1,1,1,1,0,0*02\r\n";

void setup_gps()
{
    Serial.println(TinyGPSPlus::libraryVersion());
    gpsSerial.begin(9600);
    // 重新设置波特率
    // gpsSerial.print(UBX_CFG_PRT_9600);
    // Serial.println("GPS Initialize... Please wait.");
    delay(3000);
    // // 切换波特率
    // gpsSerial.begin(9600);

    gpsSerial.print("$PCAS06,1*1A\r\n"); // 查询设备信息
    gpsSerial.print(UBX_CFG_PRT_3);
    gpsSerial.print(UBX_CFG_RATE_2HZ);
    // 保存
    // gpsSerial.print("$PCAS00*01");
    delay(1000);
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
        Serial.print((char)gpsSerial.read());
        gps.encode(gpsSerial.read());
    }

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