#include "gps.h"
#include "em_device.h"

SoftwareSerial gpsSerial(PIN_GPS_RX, PIN_GPS_TX); // RX, TX
TinyGPSPlus gps;

// NMEA0183 配置
// PCAS语句是由芯片供应商定义的NMEA专用语句

// 9600
const char *UBX_CFG_PRT_9600 =
    "$PCAS01,1*1D\r\n";
// 19200bps
const char *UBX_CFG_PRT_19200 =
    "$PCAS01,3*1B\r\n";
// 38400bps
const char *UBX_CFG_PRT_38400 =
    "$PCAS01,2*1A\r\n";
// 57600bps
const char *UBX_CFG_PRT_57600 =
    "$PCAS01,4*1E\r\n";
// 设置波特率为 115200
const char *UBX_CFG_PRT_115200 =
    "$PCAS01,5*19\r\n";

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

// PCAS03
// 格式： $PCAS03,nGGA,nGLL,nGSA,nGSV,nRMC,nVTG,nZDA,nANT,nDHV,nLPS,res1,res2,nUTC,nGST,res3,res4,res5,nTIM*CS<CR><LF>
// GGA 接收机定位数据
// GLL 地理位置经纬度
// GSA 精度因子和有效卫星
// GSV 可见卫星
// RMC 推荐的最少专用导航数据
// VTG 地面速度和方位角
// GST 接收机伪距误差统计信息
// ZDA 日期和时间
// ANT 天线高度
// LPS 卫星系统闰秒修正信息
// DHV 接收机速度信息
const char *UBX_CFG_PRT_ALL =
    "$PCAS03,1,1,1,1,1,1,1,1,0,0,,,1,1,,,,1*33\r\n";
// 只接受 RMC GGA 信息
const char *UBX_CFG_PRT_RMC =
    "$PCAS03,0,0,0,0,1,1,0,0,0,1,,,0,0,,,,0*2E\r\n";

/*
$PCAS04 设置模式

$PCAS04,3*1A 北斗和 GPS 双模
$PCAS04,1*18 单 GPS 工作模式
$PCAS04,2*1B 单北斗工作模式
*/

/*
设置 NMEA 协议类型选择。
$PCAS05,1*19
2 兼容 NMEA 4.1 以上版本
5 兼容中国交通运输信息中心的 BDS/GPS 双模协议，兼容 NMEA 2.3 以上版本，兼容NMEA4.0 协议
9 兼容单 GPS NMEA0183 协议，兼容 NMEA 2.2 版本
*/

/*
查询产品信息
$PCAS06,0*1B
*/

/*
设置重启
$PCAS10,0*1C 热启动
$PCAS10,1*1D 温启动
$PCAS10,2*1E 冷启动
$PCAS10,3*1F 出厂启动
*/

/*
$PCAS12,60*28 接收机进入待机模式，持续 60 秒后自动开机
*/

String calculateChecksum(String sentence)
{
    int checksum = 0;
    // 去除 '$' 开头部分，不包含`*`符号
    for (int i = 1; i < sentence.length(); i++)
    {
        if (sentence[i] == '*')
            break;               // 遇到 '*' 结束
        checksum ^= sentence[i]; // 字符逐个异或
    }
    // 转换为16进制表示
    char hexChecksum[3];
    sprintf(hexChecksum, "%02X", checksum);
    return String(printf("%s%s\r\n", sentence.c_str(), hexChecksum));
}

void setup_gps()
{
    Serial.println(TinyGPSPlus::libraryVersion());
    gpsSerial.begin(115200);
    // 重新设置波特率
    gpsSerial.print(UBX_CFG_PRT_9600);
    Serial.println("GPS Initialize... Please wait.");
    delay(3000);
    gpsSerial.begin(9600);
    gpsSerial.print(UBX_CFG_PRT_ALL);
    gpsSerial.print(UBX_CFG_RATE_5HZ);
    gpsSerial.print("$PCAS06,0*1B"); // 查询设备信息 $PCAS06,0*1B
    // 设置 NMEA 协议
    gpsSerial.print("$PCAS05,5*19");
    // gpsSerial.print(calculateChecksum("$PCAS05,1*").c_str());
    // gpsSerial.print(calculateChecksum("$PCAS00*").c_str()); // save setting
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
        // Serial.print((char)gpsSerial.read());
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