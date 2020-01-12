#include "clock_settings.h"
#include "string_helper.h"

#include <stdlib.h>
#include <limits.h>

ClockSettings::ClockSettings():
    m_server(nullptr),
    m_cooldown(15*60), //default 15 minutes cooldown
    m_api_key(nullptr),
    m_time_zone(nullptr)
{
}

ClockSettings::~ClockSettings()
{
    delete[] m_server;
    delete[] m_api_key;
    delete[] m_time_zone;
}

void ClockSettings::add_info(char const* key, char const* val)
{
    if(string_equals(key, "server"))
    {
        if(m_server != nullptr)
            delete[] m_server;

        m_server = val;
    }
    else if(string_equals(key, "cooldown"))
    {
        long const v = atol(val);
        delete[] val;

        if(v > UINT_MAX)
            m_cooldown = UINT_MAX;
        else
        {
            if(v < 1)
                m_cooldown = 1;
            else
                m_cooldown = (unsigned int) v;

            //transform to seconds
            m_cooldown *= 60;
        }
    }
    else if(string_equals(key, "key"))
    {
        if(m_api_key != nullptr)
            delete[] m_api_key;

        m_api_key = val;
    }
    else if(string_equals(key, "zone"))
    {
        if(m_time_zone != nullptr)
            delete[] m_time_zone;

        m_time_zone = val;
    }
    else
        delete[] val;

    delete[] key;
}
