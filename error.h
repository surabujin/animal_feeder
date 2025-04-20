/*
 * error.h
 *
 */

#ifndef ERROR_H_
#define ERROR_H_

#include <stdint.h>

#include "common.h"
#include "pulse.h"

namespace embd {

using led_pulse_t = lag::PinPulse<uint16_t, uint8_t>;

class AppErrorAlternate : public AppBase {
protected:
    uint16_t error_number;

    led_pulse_t pulse_set[2];
    uint8_t pulse_set_idx;

public:
    AppErrorAlternate(uint16_t errno);

    virtual void init();
    virtual void loop(const UptimeReference &);

protected:
    void next_pulse();
};

} // namespace embd


#endif /* ERROR_H_ */
