#ifndef SETTINGS_H
#define SETTINGS_H

#include "string_builder.h"
#include "wifi_settings.h"
#include "clock_settings.h"

class Settings
{
    private:
        Settings();
        ~Settings();

        void add_line(char const* section,
                      char const* key,
                      char const* val);

    public:
        static Settings const* load(char const* settings_filename);

        WifiSettings    wifi;
};

#endif //SETTINGS_H
