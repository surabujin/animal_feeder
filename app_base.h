/*
 * app_base.h
 *
 */

#ifndef APP_BASE_H_
#define APP_BASE_H_

#include <stdint.h>

#include "utils.h"

namespace animal_feeder {

class App;

class AppInput {
public:
    virtual ~AppInput() = default;

    virtual void loop(const embd::UptimeReference &uptime) = 0;
    virtual void button_event() = 0;
    virtual void button_long_press(bool is_start) = 0;
    virtual void wheel_event(int8_t value) = 0;

    virtual void redraw() = 0;
};

class AppState: public AppInput {
protected:
    App *app;

    uint8_t draw_flags;
    uint16_t frame_counter;
public:
    AppState(App *app);
    virtual ~AppState() = default;

    virtual void enter();
    virtual void exit();

    virtual void loop(const embd::UptimeReference &uptime);
    virtual void button_event();
    virtual void button_long_press(bool is_start);
    virtual void wheel_event(int8_t value);

    virtual void redraw();
};

} // namespace animal_feeder

#endif /* APP_BASE_H_ */
