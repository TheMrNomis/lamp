#include "wifi.h"

Wifi::Wifi(WifiSettings const& settings):
    m_wifi()
{
    for(auto it = settings.networks_cbegin(); it != settings.networks_cend(); it = it->next())
        m_wifi.addAP(it->ssid(), it->pass());
    Serial.setDebugOutput(true);
}

void Wifi::keep_alive_connection()
{
    m_wifi.run();
}

void Wifi::is_connected() const
{
    return m_wifi.status() == WL_CONNECTED;
}
