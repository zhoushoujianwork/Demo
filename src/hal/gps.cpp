#include "em_config.h"
#include <TinyGPSPlus.h>
#include <SoftwareSerial.h>

SoftwareSerial gpsSerial(PIN_GPS_RX, PIN_GPS_TX); // RX, TX
TinyGPSPlus gps;

bool is_gps_ok = false;

void setup_gps()
{
    Serial.println(TinyGPSPlus::libraryVersion());
    gpsSerial.begin(9600);
    Serial.println(F("GPS 9600 Ready"));
}

void displayInfo()
{
    Serial.print(F("Location: "));
    if (gps.location.isValid())
    {
        Serial.print(gps.location.lat(), 6);
        Serial.print(F(","));
        Serial.print(gps.location.lng(), 6);
    }
    else
    {
        Serial.print(F("INVALID"));
    }

    Serial.print(F("  Date/Time: "));
    if (gps.date.isValid())
    {
        Serial.print(gps.date.month());
        Serial.print(F("/"));
        Serial.print(gps.date.day());
        Serial.print(F("/"));
        Serial.print(gps.date.year());
    }
    else
    {
        Serial.print(F("INVALID"));
    }

    Serial.print(F(" "));
    if (gps.time.isValid())
    {
        if (gps.time.hour() < 10)
            Serial.print(F("0"));
        Serial.print(gps.time.hour());
        Serial.print(F(":"));
        if (gps.time.minute() < 10)
            Serial.print(F("0"));
        Serial.print(gps.time.minute());
        Serial.print(F(":"));
        if (gps.time.second() < 10)
            Serial.print(F("0"));
        Serial.print(gps.time.second());
        Serial.print(F("."));
        if (gps.time.centisecond() < 10)
            Serial.print(F("0"));
        Serial.print(gps.time.centisecond());
    }
    else
    {
        Serial.print(F("INVALID"));
    }

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
}