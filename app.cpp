/*
 * app.cpp
 *
 */

#include <Arduino.h>

#include "app.h"
#include "s_main.h"

#include "utils.h"
//#include "ui.h"
#include "ui_datetime.h"

namespace animal_feeder {

App::App() :
        led(LED_BUILTIN, 500, 500),
		rtc_sync_timer_action(this, rtc_sync_system_timer_period_ms),
		rtc_read_action(this, rtc_read_period_ms),
		button(this, buttonPinNumber), wheel(this, encoderS1PinNumber, encoderS2PinNumber),
        feed_screw_motor(
                feedScrewStepDriverStep, feedScrewStepDriverDir,
                feedScrewStepDriverEnable, feedScrewStepDriverReset,
                feedScrewStepDriverSleep),
        feed_screw_actuator(&feed_screw_motor),
		user_input{0} {
	screen_desc = ScreenDescriptor::get_instance();
	state = new MainState(this);
}

App::~App() {
	delete state;
}

void App::init() {
    pinMode(LED_BUILTIN, OUTPUT);

    pinMode(feedScrewStepDriverDir, OUTPUT);
    pinMode(feedScrewStepDriverStep, OUTPUT);
    pinMode(feedScrewStepDriverSleep, OUTPUT);
    pinMode(feedScrewStepDriverReset, OUTPUT);
    pinMode(feedScrewStepDriverEnable, OUTPUT);

    pinMode(encoderS1PinNumber, INPUT_PULLUP);
    pinMode(encoderS2PinNumber, INPUT_PULLUP);
    pinMode(buttonPinNumber, INPUT_PULLUP);

    rtc.begin();
    screen_desc->init();

    state->enter();
    led.start();
}

void App::loop(const UptimeReference &uptime) {
    led.loop(uptime);

    rtc_sync_timer_action.loop(this, uptime);
    rtc_read_action.loop(this, uptime);
    button.loop(uptime);
    wheel.loop(uptime);
    feed_screw_actuator.loop(uptime); // will call feed_screw_motor.loop(uptiem) inside

    state->loop(uptime);
    swap_state();
}

void App::button_event() {
    user_input[BUTTON] += 1;
    // DEBUG
    if (0 < user_input[WHEEL]) {
        feed_screw_motor.request(StepMotorDrv::DIR_CLOCKWISE, user_input[WHEEL]);
    } else {
        feed_screw_motor.request(StepMotorDrv::DIR_COUNTERCLOCKWISE, 50);
    }
    // DEBUG
    state->button_event();
    swap_state();
    redraw_status_line();
}

void App::button_long_press(bool is_start) {
    user_input[LONG_START] += static_cast<int>(is_start);
    // DEBUG
    if (is_start) {
        if (feed_screw_motor.is_sleeping()) {
            feed_screw_motor.enable();
            feed_screw_motor.wake_up();
            feed_screw_motor.clear_reset();
        } else {
            feed_screw_motor.reset();
            feed_screw_motor.sleep();
            feed_screw_motor.disable();
        }
    }
    // DEBUG
    state->button_long_press(is_start);
    swap_state();
    redraw_status_line();
}

void App::wheel_event(int8_t value) {
    user_input[WHEEL] += value;
	state->wheel_event(value);
    swap_state();
	redraw_status_line();
}

void App::redraw() {
	state->redraw();
	redraw_status_line();  // DEBUG
}

screen_t* App::get_screen() {
	return screen_desc->get_screen();
}

ScreenDescriptor* App::get_screen_descriptor() const {
	return screen_desc;
}

rtc_t* App::get_rtc() {
	return &rtc;
}

Datime& App::address_now() {
	return now;
}

void App::swap_state() {
	// TODO
}

void App::redraw_status_line() {
    screen_t *screen = get_screen();

    const int screen_width_ch =
            screen_desc->get_width_px() / ui::WidgetBase::char_width +
            (screen_desc->get_width_px() % ui::WidgetBase::char_width ? 1 : 0);
    char buff[screen_width_ch + 1];
    buff[0] = 0;
    buff[screen_width_ch] = 0;

    for (int i = 0, offs = 0; i < _USER_INPUT_EVENTS_COUNT; i++) {
        offs += snprintf(
                buff + offs, screen_width_ch - offs,
                i ? " %d" : "%d", user_input[i]);
    }

    screen->setCursorXY(0, 48);
    screen->print(feed_screw_motor.is_enabled() ? 'E' : 'e');
    screen->print(feed_screw_motor.is_reset() ? 'R' : 'r');
    screen->print(feed_screw_motor.is_sleeping() ? 'S' : 's');
    screen->setCursorXY(0, 56);
    screen->print(buff);
    screen->print(' ');
//    screen->print(static_cast<int>(button.get_state()));
//    screen->print(digitalRead(buttonPinNumber));
//    screen->print(' ');
//    screen->print(static_cast<int>(wheel.get_state()));
//    screen->print(digitalRead(encoderS1PinNumber));
//    screen->print(' ');
//    screen->print(digitalRead(encoderS1PinNumber));
}

} // namespace
