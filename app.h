/*
 * app.h
 *
 */

#ifndef APP_H_
#define APP_H_

#include <stdint.h>

#include "hw.h"
#include "periodic.h"
#include "pulse.h"
#include "encoder.h"
#include "utils.h"
#include "ui.h"
#include "ui_datetime.h"

namespace animal_feeder {

class App;
class AppState;

template<typename TTime>
class ActionBase: public lag::Periodic<TTime> {
protected:
    App *app;

    ActionBase(App*, uint16_t);

public:
    ~ActionBase() = default;

    void loop(App *app, const UptimeReference &uptime);
};

class RTCSyncSystemTimer: public ActionBase<uint32_t> {
public:
    RTCSyncSystemTimer(App*, uint16_t);
    virtual void action();
};

class RTCReadAction: public ActionBase<uint16_t> {
public:
    RTCReadAction(App*, uint16_t);
    virtual void action();
};

class ScreenRedrawAction: public ActionBase<uint16_t> {
public:
    ScreenRedrawAction(App*, uint16_t);
    virtual void action();
};

class App : public AppInput {
protected:
    static const uint8_t
        encoderS1PinNumber = 7,
        encoderS2PinNumber = 8,
        buttonPinNumber = 9;

    static const uint32_t rtc_sync_system_timer_period_ms = 600 * 1000L;
    static const int rtc_read_period_ms = 950;

	AppState *state;

	lag::PinPulse<uint16_t, uint16_t> led;
	rtc_t rtc;
	ScreenDescriptor *screen_desc;
	EncoderButton button;
	EncoderWheel wheel;

	Datime now;

	UptimeReference rtc_sync_at;

	enum USER_INPUT_EVENTS {
	    BUTTON,
	    LONG_START,
	    LONG_END,
	    WHEEL,
	    _USER_INPUT_EVENTS_COUNT
	};
	int user_input[_USER_INPUT_EVENTS_COUNT];

private:
	RTCSyncSystemTimer rtc_sync_timer_action;
	RTCReadAction rtc_read_action;

public:
	App();
	~App();

	void init();

	virtual void loop(const UptimeReference &uptime);
	virtual void button_event();
	virtual void button_long_press(bool is_start);
	virtual void wheel_event(uint8_t value);

	virtual void redraw();

	inline screen_t* get_screen();
	inline const ScreenDescriptor* get_screen_descriptor() const;

	inline rtc_t* get_rtc();

	inline Datime& address_now();

    inline void redraw_status_line();
private:
	void swap_state();
};

class AppState : public AppInput {
protected:
	App *app;

	uint8_t draw_flags;
	uint16_t frame_counter;
public:
	AppState(App *app);
	virtual ~AppState() = default;

	virtual void enter();
	virtual void exit();

	virtual void loop(const UptimeReference &uptime);
	virtual void button_event();
    virtual void button_long_press(bool is_start);
	virtual void wheel_event(uint8_t value);

	virtual void redraw();
};

class MainState : public AppState {
protected:
	static const uint16_t screen_redraw_period_ms = 500;

	UptimeReference now;

	ui::DateWidget ui_date_now; // @suppress("Type cannot be resolved")
	ui::TimeWidget ui_time_now; // @suppress("Type cannot be resolved")

private:
	ScreenRedrawAction redraw_action;

public:
	MainState(App*);

	virtual void enter() override;

	virtual void loop(const UptimeReference &uptime);
	virtual void redraw();
};

} // namespace animal_feeder

#endif /* APP_H_ */
