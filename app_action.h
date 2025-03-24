/*
 * app_action.h
 *
 */

#ifndef APP_ACTION_H_
#define APP_ACTION_H_

#include <stdint.h>

#include "periodic.h"
#include "app_base.h"

namespace animal_feeder {

template<typename TTime>
class ActionBase: public lag::Periodic<TTime> {
protected:
    App *app;

    ActionBase(App *app_ptr, TTime time_period) :
            lag::Periodic<TTime>(time_period), app(app_ptr) {
    }

public:
    ~ActionBase() = default;

    void loop(App *app_ptr, const embd::UptimeReference &uptime) {
        lag::Periodic<TTime>::loop(uptime);
    }
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

} // namespace animal_feeder

#endif /* APP_ACTION_H_ */
