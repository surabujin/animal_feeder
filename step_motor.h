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
    static const uint8_t step_pulse_time_ms = 1;
    static const unsigned int
        dir_change_lag_ms = 1,
        enable_lag_ms = 1,
        reset_lag_ms = 1,
        wake_up_lag_ms = 4;

protected:
public: // DEBUG
    enum DrvState {
        S_IDLE,
        S_SET_PINS,
        S_GENERATING,
        S_RECONFIGURE
    } state;
protected:  // DEBUG

    uint8_t dir_pin, enable_pin, reset_pin, sleep_pin;

    enum PinValueMask {
        _PIN_MASK_NONE = 0,
        _PIN_MASK_ALL = 0xff,

        PIN_MASK_DIR    = 0x01,
        PIN_MASK_ENABLE = 0x02,
        PIN_MASK_RESET  = 0x04,
        PIN_MASK_SLEEP  = 0x08
    };
    uint8_t pin_values, pin_goal;

    UptimeReference lag;
    StepPulse step_pulse;

    unsigned int required_lag_ms;
    uint16_t steps_goal;

public:
    enum DirectionSet {
        DIR_CLOCKWISE,
        DIR_COUNTERCLOCKWISE,
        DIR_UNDEFINED
    };

    StepMotorDrv(
            uint8_t step, uint8_t dir, uint8_t enable, uint8_t reset,
            uint8_t sleep);

    void loop(const UptimeReference &uptime);
private:
    void loop_set_pins(const UptimeReference &uptime);
    void loop_generating();
    void loop_reconfigure();
protected:
    void enter_reconfigure();

public:
    void request(DirectionSet dir, uint16_t steps_count);
    void stop();
    bool is_generating();
    const bool is_ready() const;

    void enable();
    void disable();
    const bool is_enabled() const;

    void reset();
    void clear_reset();
    const bool is_reset() const;

    void sleep();
    void wake_up();
    const bool is_sleeping() const;

protected:
    void write_pin(uint8_t pin, bool value, PinValueMask mask, const unsigned int lag_add);

private:
    void setup_lag(const UptimeReference &uptime);
    static uint8_t write_bits(const uint8_t bitmap, const uint8_t mask, const uint8_t value);
};

} // namespace animal_feeder

#endif /* STEP_MOTOR_H_ */
