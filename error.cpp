/*
 * error.cpp
 *
 */

#include <Arduino.h>

#include "error.h"

namespace embd {

AppErrorAlternate::AppErrorAlternate(uint16_t errno) :
        error_number(errno), pulse_set_idx(0),
        pulse_set {
            led_pulse_t(LED_BUILTIN, 400, 300, 3),
            led_pulse_t(LED_BUILTIN, 1000, 300, 3)
        } {}

void AppErrorAlternate::init() {
    pinMode(LED_BUILTIN, OUTPUT);
    pulse_set[pulse_set_idx].start();
}

void AppErrorAlternate::loop(const UptimeReference &uptime) {
    led_pulse_t *ref = &pulse_set[pulse_set_idx];
    ref->loop(uptime);
    if (! ref->is_generating()) {
        next_pulse();
    }
}

void AppErrorAlternate::next_pulse() {
    pulse_set_idx += 1;
    if (1 < pulse_set_idx)
        pulse_set_idx = 0;
    pulse_set[pulse_set_idx].start();
}

} // namespace embd
