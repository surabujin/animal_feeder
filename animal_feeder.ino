#include "Arduino.h"

#include "utils.h"
#include "app.h"

using namespace animal_feeder;

App app;
UptimeReference uptime;

void setup() {
    Serial.begin(115200);

	app.init();

/*
	Serial.begin(115200);
	Serial.println("begin A3");
*/
}

void loop() {
	uptime.update(millis());

	app.loop(uptime);
}
