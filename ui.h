/*
 * ui.h
 *
 */

#ifndef UI_H_
#define UI_H_

#include <stdint.h>

#include <GyverOLED.h>

#include "hw.h"

namespace animal_feeder {
namespace ui {

enum DrawFlags {
	DRAW_FORCE_F = 0x01
};

class Point {
protected:
	int16_t px, py;

public:
	Point(int16_t x, int16_t y);
	Point(const Point &other);

	Point& operator +(Point &other);

	bool operator ==(Point &other);

	int16_t get_px() const;
	int16_t get_py() const;

    const Point add_x(int16_t value) const;
    const Point add_y(int16_t value) const;
};

class Size2d {
protected:
	uint16_t width, height;

public:
    Size2d();
	Size2d(uint16_t w, uint16_t h);
	Size2d(const Size2d &other);

	uint16_t get_width() const;
	uint16_t get_height() const;
};

class Box {
protected:
	Point top_left;
	Size2d size;

public:
	Box(Point &tl, Size2d s);

	inline const Point& get_top_left() const;
	inline const Point get_bottom_right() const;
	inline const Size2d& get_size() const;
};

class PointPair{
protected:
    const Point first, second;

public:
    PointPair(const Point&, const Point&);
    PointPair(const PointPair&);

    PointPair& operator =(const Point&) = delete;

    const Point get_first() const;
    const Point get_second() const;
};

class WidgetBase {
protected:
    enum Flags_t {
        DIRTY_F = 0x01
    };

    uint8_t flags;

    WidgetBase();
public:
    static const uint8_t char_width = 6, char_height = 8;

	virtual ~WidgetBase() = default;

	virtual const Size2d draw(ScreenDescriptor *context, const Point &location, const uint8_t flags) = 0;

    static const PointPair draw_and_step(ScreenDescriptor *context, WidgetBase *widget, const Point &location, const uint8_t flags);
};

class TextWidget : public WidgetBase {
protected:
	const char* text;
	uint16_t length_ch;

public:
	TextWidget(const char *);

	virtual const Size2d draw(ScreenDescriptor *context, const Point &location, const uint8_t flags);
	virtual inline const Size2d get_size() const;
};

} // namespace ui
} // namespace animal_feeder

#endif /* UI_H_ */
