/*
 * pin_monitor.cpp
 *
 */

#include <Arduino.h>

#include "pin_monitor.h"

namespace hw {
namespace pin {

PinMonitor::PinMonitor(uint8_t pin) :
        PinMonitor(pin, default_unstable_time, default_long_active_time) {}

PinMonitor::PinMonitor(
        uint8_t pin, delay_t time_unstable, delay_t time_long_pulse) :
            pin_number(pin),
            time_unstable_ms(time_unstable),
            time_long_pulse_ms(time_long_pulse) {
    state = S_PASSIVE;
}

void PinMonitor::loop(const UptimeReference &uptime) {
    switch (state) {
    case S_PASSIVE:
        if (!read())
            break;
        state = S_TRANSIENT;
        lag_end_uptime.add(&uptime, time_unstable_ms);
        break;

    case S_TRANSIENT:
        if (uptime < lag_end_uptime)
            break;
        action(PASSIVE_TO_ACTIVE);
        if (read()) {
            state = S_ACTIVE;
            lag_end_uptime.add(&uptime, time_long_pulse_ms);
        } else {
            state = S_PASSIVE;
            action(ACTIVE_TO_PASSIVE);
            action(PULSE);
        }
        break;

    case S_ACTIVE:
        if (lag_end_uptime < uptime) {
            state = S_LONG_ACTIVE;
            action(LONG_PULSE_START);
            break;
        }

        if (!read()) {
            state = S_PASSIVE;
            action(ACTIVE_TO_PASSIVE);
            action(PULSE);
        }

        break;

    case S_LONG_ACTIVE:
        if (read())
            break;

        state = S_PASSIVE;
        action(ACTIVE_TO_PASSIVE);
        action(LONG_PULSE_END);
        break;
    }
}

int PinMonitor::read_hw_value() {
    return digitalRead(pin_number);
}

int PinMonitor::read_value(int value) {
    return value;
}

inline const uint8_t PinMonitor::get_pin_number() const {
    return pin_number;
}

inline int PinMonitor::read() {
    return read_value(read_hw_value());
}

NegativePinMonitor::NegativePinMonitor(uint8_t pin) :
        PinMonitor(pin) {}

NegativePinMonitor::NegativePinMonitor(
        uint8_t pin, delay_t time_unstable, delay_t long_active) :
            PinMonitor(pin, time_unstable, long_active) {}

int NegativePinMonitor::read_value(int value) {
    return !value;
}

} // namespace pin
} // namespace hw
