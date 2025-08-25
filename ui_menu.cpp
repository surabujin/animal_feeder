/*
 * ui_menu.cpp
 *
 */

#include "ui_menu.h"

namespace animal_feeder {
namespace ui {

MenuEntry::MenuEntry(const char *message) : message_len(strlen(message)) {
    this->message = message;
}

const Size2d MenuEntry::draw(ScreenDescriptor *context, const Point &location, const uint8_t flags) {
    screen_t *screen = context->get_screen();
    screen->setCursorXY(location.get_px(), location.get_py());
    screen->print(message);

    return Size2d(strlen(message) * WidgetBase::char_width, WidgetBase::char_height);
}

Menu::Menu() :
        view_v_size(0),
        entries(nullptr),
        entries_count(0), view_offset_index(0), view_active_index(0) {}

Menu::~Menu() {
    if (entries) {
        free(entries);
    }
}

Menu* Menu::add_entry(MenuEntry *subject) {
    entries = static_cast<MenuEntry**>(realloc(entries, (entries_count + 1) * sizeof(MenuEntry*)));
    // TODO: OOM handling
    entries[entries_count++] = subject;
    return this;
}

Menu* Menu::setup_view(uint8_t v_size) {
    view_v_size = v_size;
    return this;
}

const Size2d Menu::draw(ScreenDescriptor *context, const Point &location, const uint8_t flags) {
    if (! entries)
        return Size2d();

    uint16_t display_x_alloc = 0, display_y_alloc = 0;
    PointPair pivot(location, location);
    for (uint8_t idx = view_offset_index; idx < entries_count && display_y_alloc < view_v_size; idx++) {
        pivot = draw_and_step(context, entries[idx], pivot.get_second(), flags);
        display_x_alloc = max(pivot.get_first().get_px() - location.get_px(), display_x_alloc);
        display_y_alloc = pivot.get_second().get_py() - location.get_py();
    }

    return Size2d(display_x_alloc, display_y_alloc);
}

} // namespace ui
} // namespace animal_feeder

