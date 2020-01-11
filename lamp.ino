#include "settings.h"

Settings const* settings = nullptr;

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
}

void loop() {
    if(!ok) {delay(3600000); return;}
}
