/*
 * hw.h
 *
 */

#ifndef HW_H_
#define HW_H_

#include <stdint.h>

#include <GyverDS3231.h>
#include <GyverOLED.h>

#include "utils.h"

namespace animal_feeder {

typedef GyverDS3231 rtc_t;
typedef GyverOLED<SSD1306_128x64, OLED_NO_BUFFER> screen_t;

class ScreenDescriptor {
protected:
	screen_t screen;
	uint16_t width_px, height_px;

	ScreenDescriptor(uint8_t width, uint8_t height);

public:
	ScreenDescriptor() = delete;
	ScreenDescriptor(ScreenDescriptor const&) = delete;
	ScreenDescriptor& operator= (ScreenDescriptor const&) = delete;

    void init();

	static ScreenDescriptor* get_instance();

	screen_t *get_screen();
    inline const uint16_t get_width_px() const;
    inline const uint16_t get_height_px() const;
};

class AppInput {
public:
    virtual ~AppInput() = default;

    virtual void loop(const UptimeReference &uptime) = 0;
    virtual void button_event() = 0;
    virtual void button_long_press(bool is_start) = 0;
    virtual void wheel_event(int8_t value) = 0;

    virtual void redraw() = 0;
};

// ScreenDescriptor getter/setters

inline const uint16_t ScreenDescriptor::get_width_px() const {
    return width_px;
}

inline const uint16_t ScreenDescriptor::get_height_px() const {
    return height_px;
}

} // namespace

#endif /* HW_H_ */
