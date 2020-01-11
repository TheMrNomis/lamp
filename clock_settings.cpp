#include "clock_settings.h"
#include "string_helper.h"

#include <stdlib.h>
#include <limits.h>

ClockSettings::ClockSettings():
    m_server(nullptr)
{
}

ClockSettings::~ClockSettings()
{
    delete[] m_server;
}

void ClockSettings::add_info(char const* key, char const* val)
{
    if(string_equals(key, "server"))
    {
        if(m_server != nullptr)
            delete[] m_server;

        m_server = val;
    }
    else if(string_equals(key, "val"))
    {
        long const v = atoi(val);
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
    else
        delete[] val;

    delete[] key;
}
