#ifndef CLOCK_H
#define CLOCK_H

#include <WiFiUdp.h>

#include "clock_settings.h"

unsigned int const NTP_BUFFER_SIZE = 48;
unsigned int const NTP_PORT = 123;
unsigned int const TZ_RETRY_COOLDOWN = 60;

class Clock
{
    public:
        Clock(ClockSettings const& settings);

        void internet_update();

        char* print_time_UTC(time_t t) const;
        char* print_time_UTC() const;
        char* print_time_local(time_t t) const;
        char* print_time_local() const;

    protected:
        void ntp_update();
        void ntp_ask();
        void ntp_checkResponse();

        void tz_update();

    protected:
        ClockSettings const& m_settings;

        time_t    m_last_NTP_check;
        bool      m_NTP_waiting_for_response;

        WiFiUDP   m_UDP;
        IPAddress m_NTP_ip;
        uint8_t   m_NTP_buffer[NTP_BUFFER_SIZE];

        int       m_TZ_offset;
        time_t    m_next_TZ_check;
};

#endif //CLOCK_H
