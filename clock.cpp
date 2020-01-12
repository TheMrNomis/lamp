#include "clock.h"

#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>
#include <DS1307RTC.h>
#include <jsonlib.h>
#include <stdlib.h>
#include "string_builder.h"

Clock::Clock(ClockSettings const& settings):
    m_settings(settings),
    m_last_NTP_check(0),
    m_NTP_waiting_for_response(false),
    m_UDP(),
    m_NTP_ip(),
    m_TZ_offset(0),
    m_next_TZ_check(0)
{
    setSyncProvider(RTC.get);
    m_UDP.begin(NTP_PORT);
}

void Clock::internet_update()
{
    ntp_update();
    tz_update();
}

char* Clock::print_time_UTC(time_t t) const
{
    StringBuilder ret;
    ret.append_number(year(t));
    ret.append('-');
    ret.append_number(month(t));
    ret.append('-');
    ret.append_number(day(t));
    ret.append(' ');
    ret.append_number(hour(t));
    ret.append(':');
    ret.append_number(minute(t));
    ret.append(':');
    ret.append_number(second(t));

    return ret.to_string();
}

char* Clock::print_time_UTC() const
{
    return this->print_time_UTC(now());
}

char* Clock::print_time_local(time_t t) const
{
    return print_time_UTC(t + m_TZ_offset);
}

char* Clock::print_time_local() const
{
    return this->print_time_local(now());
}

void Clock::ntp_update()
{
    bool need_to_ask = false;
    if(timeStatus() != timeSet)
        need_to_ask = !m_NTP_waiting_for_response;
    else if(m_NTP_waiting_for_response)
    {
        //5 minutes timeout for the NTP response
        if((now() - m_last_NTP_check) > 5*60)
            m_NTP_waiting_for_response = false;
    }
    else
        need_to_ask = (now() - m_last_NTP_check) > m_settings.cooldown();

    if(need_to_ask) ntp_ask();
    ntp_checkResponse();
}

void Clock::ntp_ask()
{
    if(!WiFi.hostByName(m_settings.server(), m_NTP_ip))
    {
        Serial.print("DNS lookup for \"");
        Serial.print(m_settings.server());
        Serial.println("\" failed");
        return;
    }

    Serial.print("time server IP: \"");
    Serial.print(m_NTP_ip);
    Serial.println("\"");

    Serial.println("Sending NTP request...");
    //creating the NTP packet
    memset(m_NTP_buffer, 0, NTP_BUFFER_SIZE);
    m_NTP_buffer[0] = 0b11100011; //LI, version, mode
    m_UDP.beginPacket(m_NTP_ip, NTP_PORT);
    m_UDP.write(m_NTP_buffer, NTP_BUFFER_SIZE);
    m_UDP.endPacket();

    m_last_NTP_check = now();
    m_NTP_waiting_for_response = true;
}

void Clock::ntp_checkResponse()
{
    //no packet available
    if(m_UDP.parsePacket() == 0)
        return;

    m_NTP_waiting_for_response = false;

    m_UDP.read(m_NTP_buffer, NTP_BUFFER_SIZE);

    time_t NTP_time = (m_NTP_buffer[40] << 24) |
                      (m_NTP_buffer[41] << 16) |
                      (m_NTP_buffer[42] << 8)  |
                      (m_NTP_buffer[43]);

    time_t UNIX_time = NTP_time - 2208988800UL;

    m_last_NTP_check = UNIX_time;
    RTC.set(UNIX_time);
    setTime(UNIX_time);

    Serial.print("Time: ");
    Serial.println(UNIX_time);
}

void Clock::tz_update()
{
    //it's no use to do anything before we have the right UTC time
    if(timeStatus() != timeSet)
        return;

    //rate limiting
    if(now() <= m_next_TZ_check)
        return;

    Serial.println("Updating timezones");

    WiFiClient client;
    HTTPClient http;

    String url = "http://api.timezonedb.com/v2.1/get-time-zone?key=";
    url += m_settings.api_key();
    url += "&format=json&by=zone&zone=";
    url += m_settings.time_zone();

    Serial.print("Connecting to \"");
    Serial.print(url);
    Serial.println("\"");

    if(!http.begin(client, url))
    {
        Serial.println("Cannot connect");
        m_next_TZ_check = now() + TZ_RETRY_COOLDOWN;
    }
    else
    {
        int response = http.GET();
        if(response <= 0)
        {
            Serial.print("Error: ");
            Serial.println(http.errorToString(response));
            m_next_TZ_check = now() + TZ_RETRY_COOLDOWN;
        }
        else if(response == HTTP_CODE_OK || response == HTTP_CODE_MOVED_PERMANENTLY)
        {
            String payload = http.getString();

            int offset = jsonExtract(payload, "gmtOffset").toInt();
            String end = jsonExtract(payload, "zoneEnd");

            time_t const zoneEnd = atoll(end.c_str());

            Serial.print("offset: ");
            Serial.println(offset);
            Serial.print("Zone end: ");
            Serial.println(zoneEnd);

            m_TZ_offset = offset;

            time_t const time = now();
            if(time >= zoneEnd)
                m_next_TZ_check = time + TZ_RETRY_COOLDOWN;
            else if(zoneEnd - time > TZ_RETRY_COOLDOWN)
                m_next_TZ_check = zoneEnd - TZ_RETRY_COOLDOWN;
            else
                m_next_TZ_check = zoneEnd;

            Serial.print("next check: ");
            Serial.println(m_next_TZ_check);
        }
    }
}
