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
		const DatimeAccess_t *adapter_ptr, const uint8_t length) :
		subject(subject_ptr), adapter(adapter_ptr), length_ch(length) {
	cache = 0;
}

void DatimePartWidget::draw(const ScreenDescriptor *context, const Point &location, const uint8_t flags) {
	uint16_t current = this->adapter->read(subject);
	if (current == cache && !(flags & DRAW_FORCE_F)) {
		return;
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
}

inline const Size2d DatimePartWidget::get_size() const {
	return Size2d(char_width * length_ch, char_height);
}

DateTimeWidgetBase::DateTimeWidgetBase(
		const parts_arr_t& parts, const spacers_arr_t& spacers) {

	this->parts = parts;
	this->spacers = spacers;
}

void DateTimeWidgetBase::draw(const ScreenDescriptor *context, const Point &location, const uint8_t flags) {
	Point pivot = location;
    for (int pi = 0, si = 0; pi < parts.len; pi++, si++) {
		pivot = draw_and_step_h(context, &parts.reference[pi], pivot, flags);
		if (si < spacers.len) {
			pivot = draw_and_step_h(context, &spacers.reference[si], pivot, flags);
		}
	}
}

inline const Size2d DateTimeWidgetBase::get_size() const {
    uint16_t w = 0;
    for (int i = 0; i < parts.len; i++)
        w += parts.reference[i].get_size().get_width();
    for (int i = 0; i < spacers.len; i++)
        w += spacers.reference[i].get_size().get_width();
	return Size2d(w, char_height);
}

DateWidget::DateWidget(const Datime *subject_ptr) :
		DateWidget(subject_ptr, parts_arr_t(parts, parts_count), spacers_arr_t(spacers, spacers_count)) {}

DateWidget::DateWidget(const Datime *subject_ptr, const parts_arr_t &parts_ref, const spacers_arr_t &spacers_ref) :
		parts { { subject_ptr, &years_access, 4 },
				{ subject_ptr, &months_access, 2 },
				{ subject_ptr, &days_access, 2 } },
		spacers { {"."}, {"."} },
		DateTimeWidgetBase(parts_ref, spacers_ref) {}


TimeWidget::TimeWidget(const Datime *subject_ptr) :
		TimeWidget(subject_ptr, parts_arr_t(parts, parts_count), spacers_arr_t(spacers, spacers_count)) {}

TimeWidget::TimeWidget(const Datime *subject_ptr, const parts_arr_t &parts_ref, const spacers_arr_t &spacers_ref) :
		parts { { subject_ptr, &hours_access, 2 },
				{ subject_ptr, &minutes_access, 2 },
				{ subject_ptr, &seconds_access, 2 } },
		spacers { {":"}, {":"} },
		DateTimeWidgetBase(parts_ref, spacers_ref) {}

} // namespace ui
} // namespace animal_feeder
