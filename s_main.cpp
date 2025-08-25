/*
 * s_main.cpp
 *
 */

#include "s_main.h"

#include "hw.h"
#include "app.h"
#include "ui_menu.h"

namespace animal_feeder {

ui::MenuEntry me_0("mline-0"),
        me_1("mline-1"),
        me_2("mline-2"),
        me_3("mline-3"),
        me_4("mline-4"),
        me_5("mline-5"),
        me_6("mline-6"),
        me_7("mline-7"),
        me_8("mline-8"),
        me_9("mline-9"),
        me_a("mline-a"),
        me_b("mline-b"),
        me_c("mline-c"),
        me_d("mline-d"),
        me_e("mline-e"),
        me_f("mline-f");

MainState::MainState(App *app_ptr) :
        AppState(app_ptr),
        ui_date_now(&app_ptr->address_now()),
        ui_time_now(&app_ptr->address_now()),
        redraw_action(app_ptr, screen_redraw_period_ms) {
    if (! menu) {
        MainState::menu = initMenu();
        MainState::menu->setup_view(3 * ui::WidgetBase::char_height);
    }
}

void MainState::enter() {
    screen_t *screen = app->get_screen();
    screen->clear();

    draw_flags |= ui::DRAW_FORCE_F;
}

void MainState::loop(const embd::UptimeReference &uptime) {
    now = uptime;
    redraw_action.loop(app, uptime);
}

void MainState::wheel_event(int8_t value) {
    if (0 < value) {
        while (0 < value--)
            menu->event_next();
    } else {
        while (value++ < 0) {
            menu->event_prev();
        }
    }

    redraw();
}

void MainState::redraw() {
    ScreenDescriptor *context = app->get_screen_descriptor();

    ui::Point pivot(0, 0);
    pivot = ui_date_now.draw_and_step(context, &ui_date_now, pivot, draw_flags).get_first();
    pivot = pivot.add_x(ui::WidgetBase::char_width);
    ui_time_now.draw(context, pivot, draw_flags);

    pivot = ui::Point(0, 1 * ui::WidgetBase::char_height);
    menu->draw(context, pivot, draw_flags);

    if (draw_flags & ui::DRAW_FORCE_F)
        app->redraw_status_line();

    AppState::redraw();
}

ui::Menu* MainState::initMenu() {
    ui::Menu *value = new ui::Menu;
    value->add_entry(&me_0);
    value->add_entry(&me_1);
    value->add_entry(&me_2);
    value->add_entry(&me_3);
    value->add_entry(&me_4);
    value->add_entry(&me_5);
    value->add_entry(&me_6);
    value->add_entry(&me_7);
    value->add_entry(&me_8);
    value->add_entry(&me_9);
    value->add_entry(&me_a);
    value->add_entry(&me_b);
    value->add_entry(&me_c);
    value->add_entry(&me_d);
    value->add_entry(&me_e);
    value->add_entry(&me_f);

    return value;
}

ui::Menu* MainState::menu = nullptr;

} // namespace animal_feeder
