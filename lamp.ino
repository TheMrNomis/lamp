#include "settings.h"
#include "wifi.h"

Settings const* settings = nullptr;
Wifi  * wifi  = nullptr;

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

    wifi  = new Wifi(settings->wifi);
}

void loop() {
    if(!ok) {delay(3600000); return;}

    wifi->keep_alive_connection();
}
