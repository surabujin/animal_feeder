/*
 * ui_date.h
 *
 */

#ifndef UI_DATETIME_H_
#define UI_DATETIME_H_

#include <stdint.h>

#include <core/Datime.h>

#include "ui.h"
#include "dt_adapter.h"
#include "utils.h"

namespace animal_feeder {
namespace ui {

typedef utils::datetime::DatimeAccess DatimeAccess_t;

class DatimePartWidget : public WidgetBase {
protected:
	Datime *subject;

	DatimeAccess_t *adapter;
	const uint8_t length_ch;
	uint16_t cache;

public:
    DatimePartWidget(Datime*, DatimeAccess_t*, const uint8_t);

	virtual void draw(ScreenDescriptor *context, const Point &location, const uint8_t flags);
	virtual inline const Size2d get_size() const;
};

class DateTimeWidgetBase : public WidgetBase {
public:
    typedef embd::ArrReference<DatimePartWidget> parts_arr_t;
    typedef embd::ArrReference<TextWidget> spacers_arr_t;

protected:
	parts_arr_t parts;
	spacers_arr_t spacers;

	DateTimeWidgetBase() = default;
	DateTimeWidgetBase(parts_arr_t, spacers_arr_t);

public:
	virtual void draw(ScreenDescriptor *context, const Point &location, const uint8_t flags);
	virtual inline const Size2d get_size() const;
};

class DateWidget : public DateTimeWidgetBase {
	static const int parts_count = 3;
	static const int spacers_count = 2;

	DatimePartWidget parts[parts_count];
	TextWidget spacers[spacers_count];

	DateWidget(Datime *subject, parts_arr_t parts, spacers_arr_t spacers);

public:
	DateWidget(Datime *subject);
};

class TimeWidget : public DateTimeWidgetBase {
	static const int parts_count = 3;
	static const int spacers_count = 2;

	DatimePartWidget parts[parts_count];
	TextWidget spacers[spacers_count];

	TimeWidget(Datime *subject, parts_arr_t parts, spacers_arr_t spacers);

public:
	TimeWidget(Datime *subject);
};

} // namespace ui
} // namespace animal_feeder

#endif /* UI_DATETIME_H_ */
