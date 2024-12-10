/*
 * hw.cpp
 *
 */

#include <Arduino.h>

#include "hw.h"

namespace animal_feeder {

ScreenDescriptor::ScreenDescriptor(uint8_t width, uint8_t height)
		: width_px(width), height_px(height) {}

void ScreenDescriptor::init() {
	screen.init();
	screen.clear();
}

screen_t* ScreenDescriptor::get_screen() {
	return &screen;
}

ScreenDescriptor* ScreenDescriptor::get_instance() {
	static ScreenDescriptor instance(128, 64);
	return &instance;
}

} // namespace animal_feeder
