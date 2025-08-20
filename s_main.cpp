/*
 * s_main.cpp
 *
 */

#include "s_main.h"

#include "hw.h"
#include "app.h"

namespace animal_feeder {

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

void MainState::loop(const embd::UptimeReference &uptime) {
    now = uptime;
    redraw_action.loop(app, uptime);
}

void MainState::redraw() {
    ScreenDescriptor *context = app->get_screen_descriptor();

    screen_t *screen = app->get_screen();
    ui::Point pivot(6, 8);
    ui::Size2d size = ui_date_now.draw(context, pivot, draw_flags);

    uint16_t px = pivot.get_px();
    px += size.get_width();
    pivot = ui::Point(pivot.get_px() + px, pivot.get_py());
    ui_time_now.draw(context, pivot, draw_flags);

    if (draw_flags & ui::DRAW_FORCE_F)
        app->redraw_status_line();

    AppState::redraw();
}

} // namespace animal_feeder
