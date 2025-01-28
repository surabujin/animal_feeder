/*
 * app_action.cpp
 *
 */

#include "app_action.h"

#include "hw.h"
#include "app.h"

namespace animal_feeder {

RTCSyncSystemTimer::RTCSyncSystemTimer(App *app_ptr, uint16_t time_period) :
        ActionBase(app_ptr, time_period) {
}

void RTCSyncSystemTimer::action() {
    rtc_t *rtc = app->get_rtc();
    rtc->updateNow();
}

RTCReadAction::RTCReadAction(App *app_ptr, uint16_t time_period) :
        ActionBase(app_ptr, time_period) {
}

void RTCReadAction::action() {
    rtc_t *rtc = app->get_rtc();
    app->address_now() = rtc->getTime();
}

ScreenRedrawAction::ScreenRedrawAction(App *app_ptr, uint16_t time_period) :
        ActionBase(app_ptr, time_period) {
}

void ScreenRedrawAction::action() {
    app->redraw();
}

} // namespace animal_feeder
