/*
 * app.h
 *
 */

#ifndef APP_H_
#define APP_H_

#include <stdint.h>

#include "common.h"
#include "hw.h"
#include "app_base.h"
#include "app_action.h"
#include "pulse.h"
#include "encoder.h"
#include "step_motor.h"
#include "feed_screw.h"

namespace animal_feeder {

class App : public embd::AppBase, public AppInput {
protected:
    static const uint8_t  // pin mapping
        feedScrewStepDriverDir = 2,
        feedScrewStepDriverStep = 3,
        feedScrewStepDriverSleep = 4,
        feedScrewStepDriverReset = 5,
        feedScrewStepDriverEnable = 6,
        feedScrewStepDriverFail = 7,

        encoderS1PinNumber = 12,
        encoderS2PinNumber = 11,
        buttonPinNumber = 10;

    static const uint32_t rtc_sync_system_timer_period_ms = 600 * 1000L;
    static const int rtc_read_period_ms = 950;

	AppState *state;

    lag::PinPulse<uint16_t, uint16_t> led;
	rtc_t rtc;
	ScreenDescriptor *screen_desc;
	EncoderButton button;
	EncoderWheel wheel;
	StepMotorDrv feed_screw_motor;
	FeedScrewActuator feed_screw_actuator;

	Datime now;

    embd::UptimeReference rtc_sync_at;

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

    virtual void init();
    virtual void loop(const embd::UptimeReference &);

	virtual void button_event();
	virtual void button_long_press(bool is_start);
	virtual void wheel_event(int8_t value);

	virtual void redraw();

    screen_t* get_screen();
    ScreenDescriptor* get_screen_descriptor() const;

    rtc_t* get_rtc();

    Datime& address_now();

    void redraw_status_line();

private:
	void swap_state();
};

} // namespace animal_feeder

#endif /* APP_H_ */
