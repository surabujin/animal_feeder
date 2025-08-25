/*
 * s_main.h
 *
 */

#ifndef S_MAIN_H_
#define S_MAIN_H_

#include <stdint.h>

#include "utils.h"
#include "ui.h"
#include "ui_datetime.h"
#include "ui_menu.h"

#include "app_base.h"
#include "app_action.h"

namespace animal_feeder {

class MainState : public AppState {
protected:
    static const uint16_t screen_redraw_period_ms = 500;
    static ui::Menu *menu;

    embd::UptimeReference now;

    ui::DateWidget ui_date_now;
    ui::TimeWidget ui_time_now;

private:
    ScreenRedrawAction redraw_action;

    static ui::Menu* initMenu();

public:
    MainState(App*);

    virtual void enter() override;

    virtual void loop(const embd::UptimeReference &uptime);
    virtual void wheel_event(int8_t value);

    virtual void redraw();
};

} // namespace animal_feeder

#endif /* S_MAIN_H_ */
