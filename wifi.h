#ifndef WIFI_H
#define WIFI_H

#include <ESP8266WiFiMulti.h>

#include "wifi_settings.h"

class Wifi
{
    public:
        Wifi(WifiSettings const& settings);

        void keep_alive_connection();
        bool is_connected() const;

    protected:
        ESP8266WiFiMulti m_wifi;
};

#endif //WIFI_H
