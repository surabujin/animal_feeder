/*
 * encoder.h
 *
 */

#ifndef ENCODER_H_
#define ENCODER_H_

#include <stdint.h>

#include "pin_monitor.h"
#include "hw.h"

namespace animal_feeder {

typedef hw::pin::PinMonitor PinMonitor;
typedef hw::pin::NegativePinMonitor NegativePinMonitor;

class EncoderButton : public NegativePinMonitor {
protected:
    AppInput *app;

public:
    EncoderButton(AppInput*, uint8_t pin);

protected:
    virtual void action(Signal signal);
};

class EncoderWheel : public PinMonitor {
protected:
    AppInput *app;
    uint8_t pin_s2_number;

public:
    EncoderWheel(AppInput*, uint8_t pin_s1_number, uint8_t ping_s2_number);

protected:
    virtual void action(Signal signal);
};

} // namepsace animal_reader

#endif /* ENCODER_H_ */
