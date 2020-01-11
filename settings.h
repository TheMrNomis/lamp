#ifndef SETTINGS_H
#define SETTINGS_H

#include "Arduino.h"
#include "wifi_settings.h"
#include "clock_settings.h"

class Settings
{
    private:
        Settings();
        ~Settings();

        void add_line(String const& section, String const& key, String const& val);

    public:
        static Settings const* load(char const* settings_filename);

    protected:
};

#endif //SETTINGS_H
