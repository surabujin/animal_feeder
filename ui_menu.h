/*
 * ui_menu.h
 *
 */

#ifndef UI_MENU_H_
#define UI_MENU_H_

#include <stdint.h>

#include "ui.h"

namespace animal_feeder {
namespace ui {

class MenuEntry : public WidgetBase {
public:
    MenuEntry(const char *);

    const Size2d draw(ScreenDescriptor *context, const Point &location, const uint8_t flags);

protected:
    const char *message;
    const size_t message_len;
};

class Menu : public WidgetBase {
public:
    Menu();
    ~Menu();

    Menu* add_entry(MenuEntry*);
    Menu* setup_view(uint8_t v_size);

    const Size2d draw(ScreenDescriptor *context, const Point &location, const uint8_t flags);

protected:
    uint8_t view_v_size;

    MenuEntry** entries;
    uint8_t entries_count, view_offset_index, view_active_index;
};

} // namespace ui
} // namespace animal_feeder

#endif /* UI_MENU_H_ */
