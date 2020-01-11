#include "settings.h"
#include "wifi.h"
#include "clock.h"

Settings const* settings = nullptr;
Wifi  * wifi = nullptr;
Clock * clk  = nullptr;

bool ok = true;

void setup() {
    Serial.begin(9600);

    Serial.print("\nLoading settings...");

    settings = Settings::load("settings.ini");
    if(settings == nullptr)
    {
        Serial.println(" NOT OK");
        ok = false;
        return;
    }
    else
        Serial.println(" OK");

    wifi = new Wifi(settings->wifi);
    clk  = new Clock(settings->clock);
}

void loop() {
    if(!ok) {delay(3600000); return;}

    wifi->keep_alive_connection();
    if(wifi->is_connected())
        clk->internet_update();
}
