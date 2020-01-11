#include "settings.h"

Settings const* settings = NULL;

void setup() {
    Serial.begin(9600);

    Serial.print("\nLoading settings...");

    settings = Settings::load("settings.ini");
    if(settings == NULL)
        Serial.println(" NOT OK");
    else
        Serial.println(" OK");
}

void loop() {
}
