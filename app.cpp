/*
 * app.cpp
 *
 */

#include <Arduino.h>

#include "app.h"

#include "utils.h"

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
        feed_screw_actuator(feed_screw_motor),
		user_input{0} {
	screen_desc = ScreenDescriptor::get_instance();
	state = new MainState(this);
}

App::~App() {
	delete state;
}

void App::init() {
    pinMode(LED_BUILTIN, OUTPUT);

    pinMode(encoderS1PinNumber, INPUT_PULLUP);
    pinMode(encoderS2PinNumber, INPUT_PULLUP);
    pinMode(buttonPinNumber, INPUT_PULLUP);

    rtc.begin();
    screen_desc->init();

    state->enter();
    led.start();
}

void App::loop(const UptimeReference &uptime) {
	led.loop(uptime);  // TODO: убрать

    rtc_sync_timer_action.loop(this, uptime);
    rtc_read_action.loop(this, uptime);
    button.loop(uptime);
    wheel.loop(uptime);

    state->loop(uptime);
    swap_state();
}

void App::button_event() {
    user_input[BUTTON] += 1;
    state->button_event();
    swap_state();
    redraw_status_line();
}

void App::button_long_press(bool is_start) {
    user_input[LONG_START] += is_start;
    state->button_long_press(is_start);
    swap_state();
    redraw_status_line();
}

void App::wheel_event(uint8_t value) {
    user_input[WHEEL] += value;
	state->wheel_event(value);
    swap_state();
	redraw_status_line();
}

void App::redraw() {
	state->redraw();
}

inline screen_t* App::get_screen() {
	return screen_desc->get_screen();
}

inline const ScreenDescriptor* App::get_screen_descriptor() const {
	return screen_desc;
}

inline rtc_t* App::get_rtc() {
	return &rtc;
}

inline Datime& App::address_now() {
	return now;
}

void App::swap_state() {
	// TODO
}

inline void App::redraw_status_line() {
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

    screen->setCursorXY(0, 56);
    screen->print(buff);
//    screen->print(' ');
//    screen->print(static_cast<int>(button.get_state()));
//    screen->print(digitalRead(buttonPinNumber));
//    screen->print(' ');
//    screen->print(static_cast<int>(wheel.get_state()));
//    screen->print(digitalRead(encoderS1PinNumber));
//    screen->print(' ');
//    screen->print(digitalRead(encoderS1PinNumber));
}

// -- actions --

template<typename TTime>
ActionBase<TTime>::ActionBase(App *app_ptr, uint16_t time_period) :
        lag::Periodic<TTime>(time_period), app(app_ptr) {}

template<typename TTime>
void ActionBase<TTime>::loop(App *app_ptr, const UptimeReference &uptime) {
    lag::Periodic<TTime>::loop(uptime);
}

RTCSyncSystemTimer::RTCSyncSystemTimer(App *app_ptr, uint16_t time_period) :
        ActionBase(app_ptr, time_period) {}

void RTCSyncSystemTimer::action() {
	rtc_t *rtc = app->get_rtc();
	rtc->updateNow();
}

RTCReadAction::RTCReadAction(App *app_ptr, uint16_t time_period) :
        ActionBase(app_ptr, time_period) {}

void RTCReadAction::action() {
	rtc_t *rtc = app->get_rtc();
	app->address_now() = rtc->getTime();
}

ScreenRedrawAction::ScreenRedrawAction(App *app_ptr, uint16_t time_period) :
        ActionBase(app_ptr, time_period) {}

void ScreenRedrawAction::action() {
	app->redraw();
}

// -- AppState --

AppState::AppState(App *app_ptr) : app(app_ptr), frame_counter(0), draw_flags(0) {}

void AppState::enter() {
    draw_flags = ui::DRAW_FORCE_F;
}

void AppState::exit() {}

void AppState::loop(const UptimeReference &uptime) {}

void AppState::button_event() {}

void AppState::button_long_press(bool is_start) {}

void AppState::wheel_event(uint8_t value) {}

void AppState::redraw() {
    ++frame_counter;
    draw_flags &= ~ui::DRAW_FORCE_F;
}

// -- MainState --

MainState::MainState(App *app_ptr) :
		AppState(app_ptr),
		ui_date_now(&app_ptr->address_now()),
		ui_time_now(&app_ptr->address_now()),
		redraw_action(app_ptr, screen_redraw_period_ms) {}

void MainState::enter() {
	screen_t *screen = app->get_screen();
	screen->clear();

    draw_flags |= ui::DRAW_FORCE_F;
}

void MainState::loop(const UptimeReference &uptime) {
    now = uptime;
	redraw_action.loop(app, uptime);
}

void MainState::redraw() {
	const ScreenDescriptor *context = app->get_screen_descriptor();

    screen_t *screen = app->get_screen();
	ui::Point pivot(6, 8);
	ui_date_now.draw(context, pivot, draw_flags);

	uint16_t px = pivot.get_px();
	ui::Size2d size = ui_date_now.get_size();
	px += size.get_width();
	pivot = ui::Point(pivot.get_px() + px, pivot.get_py());
	ui_time_now.draw(context, pivot, draw_flags);

    screen->setCursorXY(0, 40);
    screen->print(now.uptime);
    screen->print(' ');
    screen->print(now.loop);

    if (draw_flags & ui::DRAW_FORCE_F)
	    app->redraw_status_line();

    AppState::redraw();
}

} // namespace
