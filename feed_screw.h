/*
 * feed_screw.h
 *
 */

#ifndef FEED_SCREW_H_
#define FEED_SCREW_H_

#include <stdint.h>

#include "step_motor.h"
#include "utils.h"

namespace animal_feeder {

class FeedScrewActuator {
public:
    static const uint16_t
        iteration_forward_dfl_steps = 20,
        iteration_reverse_dfl_steps = 5;

protected:
    enum StateSet {
        S_IDLE,
        S_CHARGED,
        S_FORWARD_REVOLUTION,
        S_REVERSE_REVOLUTION
    } state;
    enum FlagBits {
        F_IGNORE_ITERATION = 0x01
    };
    uint8_t flags;

    StepMotorDrv *driver;

    uint16_t iteration_forward_steps;
    uint16_t iteration_reverse_steps;

    uint16_t goal_steps;
    uint16_t iteration;
public:
    FeedScrewActuator(
            StepMotorDrv *ext_driver,
            uint16_t forward_steps = iteration_reverse_dfl_steps,
            uint16_t reverse_steps = iteration_reverse_dfl_steps);

    void loop(const UptimeReference &uptime);

    void control(const uint16_t steps);
    const bool is_active() const;

    void plan_iteration();
    void swap_direction(bool is_forward_now);
    uint16_t calc_iteration_steps();
private:
    void inc_iteration();
};

} // namespace animal_feeder

#endif /* FEED_SCREW_H_ */
