/*
 * pin_monitor.h
 *
 */

#ifndef PIN_MONITOR_H_
#define PIN_MONITOR_H_

#import <stdint.h>

#include "utils.h"

namespace hw {
namespace pin {

typedef uint16_t delay_t;

class PinMonitor {
protected:
    static const delay_t default_unstable_time = 50;
    static const delay_t default_long_active_time = 1000;

    enum State {
        S_PASSIVE,
        S_TRANSIENT,
        S_ACTIVE,
        S_LONG_ACTIVE
    } state;

    uint8_t pin_number;
    delay_t time_unstable_ms;
    delay_t time_long_pulse_ms;

    UptimeReference lag_end_uptime;

public:
    enum Signal {
        PASSIVE_TO_ACTIVE,
        ACTIVE_TO_PASSIVE,
        PULSE,
        LONG_PULSE_START,
        LONG_PULSE_END
    };

    PinMonitor(uint8_t pin);
    PinMonitor(uint8_t pin, delay_t time_unstable, delay_t long_active);
    virtual ~PinMonitor() = default;

    void loop(const UptimeReference &uptime);

    inline const uint8_t get_pin_number() const;

protected:
    virtual int read_hw_value();
    virtual int read_value(int value);
    virtual void action(Signal signal) = 0;

private:
    inline int read();
};

class NegativePinMonitor : public PinMonitor {
public:
    NegativePinMonitor(uint8_t pin);
    NegativePinMonitor(uint8_t pin, delay_t time_unstable, delay_t long_active);
protected:
    virtual int read_value(int value);
};

} // namespace pin
} // namespace hw

#endif /* PIN_MONITOR_H_ */
