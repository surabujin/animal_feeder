#include "Arduino.h"

#include "utils.h"
#include "app.h"

using namespace animal_feeder;

embd::AppBase *current;
embd::UptimeReference uptime;

App app;

void setup() {
//    Serial.begin(115200);
//    Serial.println("init()");
    current = &app;
    current->init();
}

void loop() {
    uptime.update(millis());
    current->loop(uptime);
}
