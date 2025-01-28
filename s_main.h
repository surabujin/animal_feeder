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

#include "app_base.h"
#include "app_action.h"

namespace animal_feeder {

class MainState : public AppState {
protected:
    static const uint16_t screen_redraw_period_ms = 500;

    UptimeReference now;

    ui::DateWidget ui_date_now;
    ui::TimeWidget ui_time_now;

private:
    ScreenRedrawAction redraw_action;

public:
    MainState(App*);

    virtual void enter() override;

    virtual void loop(const UptimeReference &uptime);
    virtual void redraw();
};

} // namespace animal_feeder

#endif /* S_MAIN_H_ */
