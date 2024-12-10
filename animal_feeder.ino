#include "Arduino.h"

#include "utils.h"
#include "app.h"

using namespace animal_feeder;

App app;
UptimeReference uptime;

void setup() {
//    Serial.begin(115200);
//    Serial.println("init()");

    app.init();
}

void loop() {
    uptime.update(millis());
    app.loop(uptime);
}
