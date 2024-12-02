/*
 * step_motor.cpp
 *
 */

#include "step_motor.h"

namespace animal_feeder {

StepPulse::StepPulse(uint8_t pin, uint8_t pulse_time_ms) :
        lag::PinPulse<uint8_t, uint16_t>(pin, pulse_time_ms, pulse_time_ms) {}

StepMotorDrv::StepMotorDrv(uint8_t step, uint8_t dir, uint8_t enable,
        uint8_t reset, uint8_t sleep) :
        step_pulse(step, step_pulse_time_ms), dir_pin(dir), enable_pin(enable),
        reset_pin(reset), sleep_pin(sleep) {
    pin_values = 0;
}

void StepMotorDrv::loop(const UptimeReference &uptime) {
    step_pulse.loop(uptime);
    // TODO lag
}

void StepMotorDrv::request(RotateDirection dir, uint16_t steps_count) {
    RotateDirection dir_now =
            read_pin(dir_pin, PIN_MASK_DIR) ?
                    ROTATE_COUNTERCLOCKWISE : ROTATE_CLOCKWISE;
    if (dir_now != dir) {
        write_pin(dir_pin, static_cast<bool>(dir), PIN_MASK_DIR);
        // TODO lag
    }

    step_pulse.set_limit(steps_count);
    step_pulse.start();
    // TODO

}

} // namespace animal_feeder

