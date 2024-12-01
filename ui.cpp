/*
 * ui.cpp
 *
 */

#include "ui.h"

namespace animal_feeder {
namespace ui {

Point::Point(int16_t x, int16_t y) {
	px = x; py = y;
}

Point::Point(const Point &other) {
	px = other.px; py = other.py;
}

Point& Point::operator +(Point &other) {
	Point p(px + other.px, py + other.py);
	return p;
}

bool Point::operator ==(Point &other) {
	return px == other.px && py == other.py;
}

int16_t Point::get_px() const {
	return px;
}

int16_t Point::get_py() const {
	return py;
}

const uint8_t WidgetBase::char_width = 6;
const uint8_t WidgetBase::char_height = 8;

Size2d::Size2d(uint16_t w, uint16_t h) : width(w), height(h) {}

Size2d::Size2d(const Size2d &other) {
	width = other.width;
	height = other.height;
}

uint16_t Size2d::get_height() const {
	return height;
}

uint16_t Size2d::get_width() const {
	return width;
}

Box::Box(Point &tl, Size2d s) : top_left(tl), size(s) {}

inline const Point& Box::get_top_left() const {
	return top_left;
}

inline const Point Box::get_bottom_right() const {
	return Point(1, 2);
//	return Point(top_left.get_px() + size.get_width(), top_left.get_py() + size.get_height());
}

inline const Size2d& Box::get_size() const {
	return size;
}

const Point WidgetBase::draw_and_step_h(
		const ScreenDescriptor *context, WidgetBase *widget, const Point &location, const uint8_t flags) {
	widget->draw(context, location, flags);
	Size2d size = widget->get_size();
	return Point(location.get_px() + size.get_width(), location.get_py());
}

TextWidget::TextWidget(const char *content) : text(content), flags(0) {
	length_ch = strlen(text);
}

void TextWidget::draw(const ScreenDescriptor *context, const Point &location, const uint8_t flags) {
	if (!(flags & DRAW_FORCE_F) && (this->flags & VISIBLE_F)) {
		return;
	}

	screen_t *screen = context->get_screen();
	screen->setCursorXY(location.get_px(), location.get_py());
	screen->print(text);
	this->flags |= VISIBLE_F;
}

inline const Size2d TextWidget::get_size() const {
	return Size2d(length_ch * char_width, char_height);
}

} // namespace ui
} // namespace animal_feeder
