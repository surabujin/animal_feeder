/*
 * feed_screw.cpp
 *
 */
#include <Arduino.h>

#include "feed_screw.h"

namespace animal_feeder {

FeedScrewActuator::FeedScrewActuator(StepMotorDrv &ext_driver,
        uint16_t forward_steps, uint16_t reverse_steps) :
        driver(ext_driver), state(S_IDLE) {
    flags = 0;

    iteration_forward_steps = forward_steps;
    iteration_reverse_steps = reverse_steps;

    goal_steps = 0;
    iteration = 0;
}

void FeedScrewActuator::loop(const UptimeReference &uptime) {
    driver.loop(uptime);

    switch (state) {
    case S_CHARGED:
        plan_iteration();
        break;

    case S_FORWARD_REVOLUTION:
    case S_REVERSE_REVOLUTION:
        if (! driver.is_generating())
            swap_direction(state == S_FORWARD_REVOLUTION);
        break;
    }
}

void FeedScrewActuator::control(const uint16_t steps) {
    goal_steps = steps;
    iteration = 0;

    switch (state) {
    case S_IDLE:
        state = S_CHARGED;
        break;
    case S_FORWARD_REVOLUTION:
    case S_REVERSE_REVOLUTION:
        flags |= F_IGNORE_ITERATION;
        break;
    }
}

const bool FeedScrewActuator::is_active() const {
    return state != S_IDLE;
}

void FeedScrewActuator::plan_iteration() {
    uint16_t steps = calc_iteration_steps();
    if (0 < steps) {
        state = S_FORWARD_REVOLUTION;
        driver.request(StepMotorDrv::DIR_CLOCKWISE, steps);
    } else {
        state = S_IDLE;
    }
}

void FeedScrewActuator::swap_direction(bool is_forward_now) {
    uint16_t iteration_rollback = iteration;
    inc_iteration();

    uint16_t steps = calc_iteration_steps();
    StepMotorDrv::DirectionSet dir;

    if (is_forward_now) {
        iteration = iteration_rollback;
        dir = StepMotorDrv::DIR_COUNTERCLOCKWISE;
        if (steps)
            steps = iteration_reverse_steps;
    } else {
        dir = StepMotorDrv::DIR_CLOCKWISE;
    }

    if (! steps) {
        state = S_IDLE;
        return;
    }

    driver.request(dir, steps);
    state = is_forward_now ? S_REVERSE_REVOLUTION : S_FORWARD_REVOLUTION;
}

uint16_t FeedScrewActuator::calc_iteration_steps() {
    uint16_t passed_steps = iteration * iteration_forward_steps;
    if (goal_steps <= passed_steps)
        return 0;
    return min(iteration_forward_steps, goal_steps - passed_steps);
}

void FeedScrewActuator::inc_iteration() {
    if (! (flags & F_IGNORE_ITERATION))
        iteration += 1;
    flags &= ~F_IGNORE_ITERATION;
}

} // namespace animal_feeder
