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

void Menu::event_next() {
    if (! entries_count)
        return;

    view_active_index = min(view_active_index + 1, entries_count - 1);
    uint8_t seen_entries = view_v_size / char_height; // TODO: true height calculation
    if (view_v_size % char_height)
        seen_entries += 1;
    if (! seen_entries)
        return;
    if (view_offset_index + seen_entries <= view_active_index) {
        view_offset_index = view_active_index - (seen_entries - 1);
    }
}

void Menu::event_prev() {
    if (0 < view_active_index) {
        view_active_index -= 1;
        view_offset_index = min(view_offset_index, view_active_index);
    }
}

const Size2d Menu::draw(ScreenDescriptor *context, const Point &location, const uint8_t flags) {
    if (! entries)
        return Size2d();

    uint16_t display_x_alloc = 0, display_y_alloc = 0;
    uint8_t marker_width = char_width + 2;
    Point pivot = location;
    const char *marks_active[2] = {">", "<"};
    const char *marks_passive[2] = {" ", " "};
    for (uint8_t idx = view_offset_index; idx < entries_count && display_y_alloc < view_v_size; idx++) {
        PointPair step(draw_and_step(context, entries[idx], pivot.add_x(marker_width), flags));

        const char **marks = idx == view_active_index ? marks_active : marks_passive;
        screen_t *screen = context->get_screen();
        screen->setCursorXY(0, pivot.get_py());
        screen->print(marks[0]);
        screen->setCursorXY(context->get_width_px() - char_width, pivot.get_py());
        screen->print(marks[1]);

        pivot = Point(location.get_px(), step.get_second().get_py());

        display_x_alloc = max(step.get_first().get_px() - location.get_px(), display_x_alloc);
        display_y_alloc = step.get_second().get_py() - location.get_py();
    }

    return Size2d(display_x_alloc, display_y_alloc);
}

} // namespace ui
} // namespace animal_feeder

