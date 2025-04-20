#include "Arduino.h"

#include "utils.h"
#include "app.h"
#include "error.h"

using namespace animal_feeder;

embd::AppBase *current;
embd::UptimeReference uptime;

App app;

void setup() {
//    Serial.begin(115200);
//    Serial.println("init()");
    current = &app;
//    current = new embd::AppErrorAlternate(42);
    current->init();
}

void loop() {
    uptime.update(millis());
    current->loop(uptime);
}
