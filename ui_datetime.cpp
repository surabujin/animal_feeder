/*
 * ui_date.cpp
 *
 */

#include <stdlib.h>

#include "ui_datetime.h"

namespace animal_feeder {
namespace ui {

utils::datetime::DatimeYearsAccess years_access;
utils::datetime::DatimeMonthsAccess months_access;
utils::datetime::DatimeDaysAccess days_access;

utils::datetime::DatimeHoursAccess hours_access;
utils::datetime::DatimeMinutesAccess minutes_access;
utils::datetime::DatimeSecondsAccess seconds_access;

DatimePartWidget::DatimePartWidget(Datime *subject_ptr,
        DatimeAccess_t *adapter_ptr, const uint8_t length) :
        subject(subject_ptr), adapter(adapter_ptr), length_ch(length) {
    cache = 0;
}

const Size2d DatimePartWidget::draw(ScreenDescriptor *context, const Point &location, const uint8_t flags) {
    const Size2d screen_size(char_width * length_ch, char_height);

    uint16_t current = this->adapter->read(subject);
	if (current == cache && !(flags & DRAW_FORCE_F)) {
        return screen_size;
	}
	cache = current;

    const int flen = 8;
    int offs = 0;
    char format[flen];
    format[offs++] = '%';
    format[offs++] = '0'; // "%0" + 3(int8_t:0-255) + "d\0"
    offs += snprintf(format + offs, flen - offs, "%d", length_ch);
    format[offs++] = 'd';
    format[offs] = 0;

    char buff[length_ch + 1]; // extra 0 char
    snprintf(buff, static_cast<int>(length_ch + 1), format, current);

    screen_t *screen = context->get_screen();
    screen->setCursorXY(location.get_px(), location.get_py());
    screen->print(buff);

    return screen_size;
}

DateTimeWidgetBase::DateTimeWidgetBase(parts_arr_t parts, spacers_arr_t spacers) {
	this->parts = parts;
	this->spacers = spacers;
}

const Size2d DateTimeWidgetBase::draw(ScreenDescriptor *context, const Point &location, const uint8_t flags) {
    PointPair step(location, location);
    int size_y;
	for (int pi = 0, si = 0; pi < parts.len; pi++, si++) {
        step = draw_and_step(context, &parts.reference[pi], step.get_first(), flags);
        size_y = max(size_y, step.get_second().get_py() - location.get_py());
		if (si < spacers.len) {
            step = draw_and_step(context, &spacers.reference[si], step.get_first(), flags);
            size_y = max(size_y, step.get_second().get_py() - location.get_py());
		}
	}

	return Size2d(step.get_first().get_px() - location.get_px(), size_y);
}

DateWidget::DateWidget(Datime *subject_ptr) :
		DateWidget(subject_ptr, parts_arr_t(parts, parts_count), spacers_arr_t(spacers, spacers_count)) {}

DateWidget::DateWidget(Datime *subject_ptr, parts_arr_t parts_ref, spacers_arr_t spacers_ref) :
		parts { { subject_ptr, &years_access, 4 },
				{ subject_ptr, &months_access, 2 },
				{ subject_ptr, &days_access, 2 } },
		spacers { {"."}, {"."} },
		DateTimeWidgetBase(parts_ref, spacers_ref) {}


TimeWidget::TimeWidget(Datime *subject_ptr) :
		TimeWidget(subject_ptr, parts_arr_t(parts, parts_count), spacers_arr_t(spacers, spacers_count)) {}

TimeWidget::TimeWidget(Datime *subject_ptr, parts_arr_t parts_ref, spacers_arr_t spacers_ref) :
		parts { { subject_ptr, &hours_access, 2 },
				{ subject_ptr, &minutes_access, 2 },
				{ subject_ptr, &seconds_access, 2 } },
		spacers { {":"}, {":"} },
		DateTimeWidgetBase(parts_ref, spacers_ref) {}

} // namespace ui
} // namespace animal_feeder
