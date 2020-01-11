#include "clock_settings.h"
#include "string_helper.h"

#include <stdlib.h>
#include <limits.h>

ClockSettings::ClockSettings()
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

        if(v < 1)
            m_cooldown = 1;
        else if(v > UINT_MAX)
            m_cooldown = UINT_MAX;
        else
            m_cooldown = (unsigned int) v;
    }
    else
        delete[] val;

    delete[] key;
}
