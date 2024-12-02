/*
 * step_motor.h
 *
 */

#ifndef STEP_MOTOR_H_
#define STEP_MOTOR_H_

#include <stdint.h>

#include "pulse.h"
#include "utils.h"

namespace animal_feeder {

class StepPulse : public lag::PinPulse<uint8_t, uint16_t> {
public:
    StepPulse(uint8_t pin, uint8_t pulse_time_ms);
};

class StepMotorDrv {
public:
    static const uint8_t step_pulse_time_ms = 2;

protected:
    enum PinValueMask {
        PIN_MASK_DIR    = 0x01,
        PIN_MASK_ENABLE = 0x02,
        PIN_MASK_RESET  = 0x04,
        PIN_MASK_SLEEP  = 0x08
    };

    uint8_t dir_pin, enable_pin, reset_pin, sleep_pin;
    uint8_t pin_values;

    UptimeReference lag_till_uptime;
    StepPulse step_pulse;

public:
    enum RotateDirection {
        ROTATE_CLOCKWISE,
        ROTATE_COUNTERCLOCKWISE,
        ROTATE_UNDEFINED
    };

    StepMotorDrv(
            uint8_t step, uint8_t dir, uint8_t enable, uint8_t reset,
            uint8_t sleep);

    void loop(const UptimeReference &uptime);

    void request(RotateDirection dir, uint16_t steps_count);

    void enable();
    void disable();

    void reset();
    void clear_reset();

    void sleep();
    void wake_up();

protected:
    bool read_pin(uint8_t pin, PinValueMask mask);
    void write_pin(uint8_t pin, bool value, PinValueMask mask);
};

} // namespace animal_feeder

#endif /* STEP_MOTOR_H_ */
