/*
 * app_base.cpp
 *
 */

#include "app_base.h"

#include "ui.h"

namespace animal_feeder {

AppState::AppState(App *app_ptr) :
        app(app_ptr), frame_counter(0), draw_flags(0) {
}

void AppState::enter() {
    draw_flags = ui::DRAW_FORCE_F;
}

void AppState::exit() {
}

void AppState::loop(const UptimeReference &uptime) {
}

void AppState::button_event() {
}

void AppState::button_long_press(bool is_start) {
}

void AppState::wheel_event(int8_t value) {
}

void AppState::redraw() {
    ++frame_counter;
    draw_flags &= ~ui::DRAW_FORCE_F;
}

} // namespace animal_feeder
