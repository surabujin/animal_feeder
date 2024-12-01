/*
 * encoder.cpp
 *
 */

#include <Arduino.h>

#include "encoder.h"

namespace animal_feeder {

EncoderButton::EncoderButton(AppInput *app_ptr, uint8_t pin) :
        NegativePinMonitor(pin), app(app_ptr) {}

void EncoderButton::action(Signal signal) {
    if (PULSE != signal)
        app->button_event();
    else if (LONG_PULSE_START)
        app->button_long_press(true);
    else if (LONG_PULSE_END)
        app->button_long_press(false);
}

EncoderWheel::EncoderWheel(
        AppInput *app_ptr, uint8_t pin_s1_number, uint8_t pin_s2_number) :
        PinMonitor(pin_s1_number), app(app_ptr), pin_s2_number(pin_s2_number) {}

void EncoderWheel::action(Signal signal) {
    if (ACTIVE_TO_PASSIVE != signal)
        return;

    int second = read_value(digitalRead(pin_s2_number));
    app->wheel_event(second ? 1 : -1);
}

} // namespace animal_feeder
