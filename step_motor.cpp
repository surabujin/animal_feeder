/*
 * step_motor.cpp
 *
 */

#include <Arduino.h>

#include "step_motor.h"

namespace animal_feeder {

StepPulse::StepPulse(uint8_t pin, uint8_t pulse_time_ms) :
        lag::PinPulse<uint8_t, uint16_t>(pin, pulse_time_ms, pulse_time_ms) {}

StepMotorDrv::StepMotorDrv(uint8_t step, uint8_t dir, uint8_t enable,
        uint8_t reset, uint8_t sleep) :
        step_pulse(step, step_pulse_time_ms), dir_pin(dir), enable_pin(enable),
        reset_pin(reset), sleep_pin(sleep) {
    state = S_IDLE;
    pin_values = pin_goal = 0;
    required_lag_ms = 0;
    steps_goal = 0;
}

void StepMotorDrv::loop(const embd::UptimeReference &uptime) {
    switch (state) {
    case S_SET_PINS:
        loop_set_pins(uptime);
        break;
    case S_GENERATING:
        loop_generating();
        break;
    case S_RECONFIGURE:
        loop_reconfigure();
        break;
    }

    step_pulse.loop(uptime);
}

void StepMotorDrv::loop_set_pins(const embd::UptimeReference &uptime) {
    if (pin_values != pin_goal) {
        uint8_t diff = pin_values ^ pin_goal;

        if (diff & PIN_MASK_DIR)
            write_pin(dir_pin, pin_goal & PIN_MASK_DIR, PIN_MASK_DIR, dir_change_lag_ms);
        if (diff & PIN_MASK_ENABLE)
            write_pin(enable_pin, pin_goal & PIN_MASK_ENABLE, PIN_MASK_ENABLE, enable_lag_ms);
        if (diff & PIN_MASK_RESET)
            write_pin(reset_pin, pin_goal & PIN_MASK_RESET, PIN_MASK_RESET, reset_lag_ms);
        if (diff & PIN_MASK_SLEEP)
            write_pin(sleep_pin, pin_goal & PIN_MASK_SLEEP, PIN_MASK_SLEEP, wake_up_lag_ms);

        pin_values = pin_goal;  // must work without this line
        setup_lag(uptime);
    }

    if (uptime < lag) {
        return;
    }

    if (steps_goal) {
        state = S_GENERATING;

        step_pulse.set_limit(steps_goal);
        step_pulse.start();
        steps_goal = 0;
    } else {
        state = S_IDLE;
    }
}

void StepMotorDrv::loop_generating() {
    if (step_pulse.is_generating())
        return;
    state = S_IDLE;
}

const bool StepMotorDrv::is_ready() const {
    return (pin_values & (PIN_MASK_ENABLE | PIN_MASK_RESET | PIN_MASK_SLEEP)) == (PIN_MASK_RESET | PIN_MASK_SLEEP);
}

void StepMotorDrv::loop_reconfigure() {
    if (step_pulse.is_generating())
        return;
    state = S_SET_PINS;
}

void StepMotorDrv::enter_reconfigure() {
    state = S_RECONFIGURE;
    step_pulse.stop();
}

void StepMotorDrv::request(DirectionSet dir, uint16_t steps_count) {
    pin_goal = write_bits(pin_goal, PIN_MASK_DIR, static_cast<bool>(dir) ? _PIN_MASK_ALL : _PIN_MASK_NONE);
    steps_goal = steps_count;

    switch (state) {
    case S_IDLE:
        state = S_SET_PINS;
        break;

    case S_SET_PINS:
    case S_RECONFIGURE:
        break;

    case S_GENERATING:
        if (pin_values == pin_goal) {
            enter_reconfigure();
        } else {
            step_pulse.set_limit(step_pulse.get_iteration() + steps_goal);
        }
        break;
    }
}

void StepMotorDrv::stop() {
    steps_goal = 0;
    enter_reconfigure();
}

bool StepMotorDrv::is_generating() {
    return state != S_IDLE;
}

void StepMotorDrv::enable() {
    pin_goal = write_bits(pin_goal, PIN_MASK_ENABLE, _PIN_MASK_NONE);
    enter_reconfigure();
}

void StepMotorDrv::disable() {
    pin_goal = write_bits(pin_goal, PIN_MASK_ENABLE, _PIN_MASK_ALL);  // reverse control
    enter_reconfigure();
}

const bool StepMotorDrv::is_enabled() const {
    return (pin_values & PIN_MASK_ENABLE) == 0;
}

void StepMotorDrv::reset() {
    pin_goal = write_bits(pin_goal, PIN_MASK_RESET, _PIN_MASK_NONE);  // reverse control
    enter_reconfigure();
}
void StepMotorDrv::clear_reset() {
    pin_goal = write_bits(pin_goal, PIN_MASK_RESET, _PIN_MASK_ALL);
    enter_reconfigure();
}

const bool StepMotorDrv::is_reset() const {
    return (pin_values & PIN_MASK_RESET) == 0;
}

void StepMotorDrv::sleep() {
    pin_goal = write_bits(pin_goal, PIN_MASK_SLEEP, _PIN_MASK_NONE);  // reverse control
    enter_reconfigure();
}
void StepMotorDrv::wake_up() {
    pin_goal = write_bits(pin_goal, PIN_MASK_SLEEP, _PIN_MASK_ALL);
    enter_reconfigure();
}

const bool StepMotorDrv::is_sleeping() const {
    return (pin_values & PIN_MASK_SLEEP) == 0;
}

void StepMotorDrv::setup_lag(const embd::UptimeReference &uptime) {
    if (! required_lag_ms)
        return;

    if (lag < uptime)
        lag.add(&uptime, required_lag_ms);
    else
        lag.add(required_lag_ms);
    required_lag_ms = 0;
}

void StepMotorDrv::write_pin(uint8_t pin, bool value, PinValueMask mask, const unsigned int lag_add) {
    digitalWrite(pin, value ? HIGH : LOW);
    pin_values = write_bits(pin_values, mask, value ? _PIN_MASK_ALL : _PIN_MASK_NONE);
    required_lag_ms += lag_add;
}

uint8_t StepMotorDrv::write_bits(const uint8_t bitmap, const uint8_t mask, const uint8_t value) {
    uint8_t result = bitmap & ~mask;
    return result | (value & mask);
}

} // namespace animal_feeder

