#include "clock.h"

#include <ESP8266WiFi.h>
#include <DS1307RTC.h>

Clock::Clock(ClockSettings const& settings):
    m_settings(settings),
    m_last_NTP_check(0),
    m_NTP_waiting_for_response(false),
    m_UDP(),
    m_NTP_ip()
{
    setSyncProvider(RTC.get);
    m_UDP.begin(NTP_PORT);
}

void Clock::internet_update()
{
    ntp_update();
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
