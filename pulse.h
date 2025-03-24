/*
 * pulse.h
 *
 */

#ifndef PULSE_H_
#define PULSE_H_

#include <stdint.h>

#ifdef AVR
#include <Arduino.h>
#else
#include <iostream>
#endif

#include "utils.h"

namespace lag {

using embd::UptimeReference;

template<typename TTime, typename TCount>
class Pulse {
	enum Flags {
		F_STOPPING = 0x01
	};

	enum State {
		S_IDLE,
		S_PENDING_START,
		S_ACTIVE,
		S_PASSIVE
	};

protected:
	uint8_t flags;
	TTime time_active, time_passive;
	TCount limit, iteration;

	UptimeReference state_end_uptime;
	State state;

public:
	Pulse(TTime time_active, TTime time_passive, TCount limit = 0);
	virtual ~Pulse() = default;

	virtual void start();
	virtual void stop();

	virtual void loop(const UptimeReference &uptime);

	uint8_t get_pin();

	bool is_generating();

	TTime get_time_active();
	void set_time_active(TTime value);

	TTime get_time_passive();
	void set_time_passwive(TTime value);

	TCount get_limit();
	void set_limit(TCount value);

	TCount get_iteration();

protected:
	virtual void transition_to_active(const UptimeReference &uptime);
	virtual void transition_to_passive(const UptimeReference &uptime);

	virtual void apply(uint8_t value) = 0;
	virtual void complete();
};

template<typename TTime, typename TCount>
Pulse<TTime, TCount>::Pulse(
        TTime active, TTime passive, TCount limit_cycles) :
        time_active(active), time_passive(passive), limit(limit_cycles) {
    flags = 0;
    state = S_IDLE;
}

template<typename TTime, typename TCount>
void Pulse<TTime, TCount>::start() {
	if (state == S_IDLE) {
		state = S_PENDING_START;
	}
	flags &= ~F_STOPPING;
}

template<typename TTime, typename TCount>
void Pulse<TTime, TCount>::stop() {
    if (state != S_IDLE)
        flags |= F_STOPPING;
}

template<typename TTime, typename TCount>
void Pulse<TTime, TCount>::loop(const UptimeReference &uptime) {
	switch(state) {
	case S_IDLE:
		break;
	case S_PENDING_START:
        iteration = 0;
		transition_to_active(uptime);
		break;
	case S_ACTIVE:
		if (state_end_uptime < uptime) {
			transition_to_passive(uptime);
		}
		break;
	case S_PASSIVE:
		if (state_end_uptime < uptime) {
			transition_to_active(uptime);
		}
		break;
	}
}

template<typename TTime, typename TCount>
void Pulse<TTime, TCount>::transition_to_active(const UptimeReference &uptime) {
	if (flags & F_STOPPING) {
		state = S_IDLE;
		flags &= ~F_STOPPING;
		complete();
		return;
	}

	if (limit && limit <= iteration) {
		state = S_IDLE;
		complete();
		return;
	}

	iteration += 1;
	state = S_ACTIVE;
	state_end_uptime.add(&uptime, time_active);
	apply(1);
}

template<typename TTime, typename TCount>
void Pulse<TTime, TCount>::transition_to_passive(const UptimeReference &uptime) {
	state = S_PASSIVE;
	state_end_uptime.add(&uptime, time_passive);
	apply(0);
}

template<typename TTime, typename TCount>
inline bool Pulse<TTime, TCount>::is_generating() {
	return state != S_IDLE;
}

template<typename TTime, typename TCount>
inline TTime Pulse<TTime, TCount>::get_time_active() {
	return time_active;
}

template<typename TTime, typename TCount>
inline void Pulse<TTime, TCount>::set_time_active(TTime value) {
	time_active = value;
}

template<typename TTime, typename TCount>
inline TTime Pulse<TTime, TCount>::get_time_passive() {
	return time_passive;
}

template<typename TTime, typename TCount>
inline void Pulse<TTime, TCount>::set_time_passwive(TTime value) {
	time_passive = value;
}

template<typename TTime, typename TCount>
inline TCount Pulse<TTime, TCount>::get_limit() {
	return limit;
}

template<typename TTime, typename TCount>
inline void Pulse<TTime, TCount>::set_limit(TCount value) {
	limit = value;
}

template<typename TTime, typename TCount>
inline TCount Pulse<TTime, TCount>::get_iteration() {
	return iteration;
}

template<typename TTime, typename TCount>
inline void Pulse<TTime, TCount>::complete() {}

template<typename TTime, typename TCount>
class PinPulse : public Pulse<TTime, TCount> {
protected:
    uint8_t pin_number;

public:
    PinPulse(uint8_t pin, TTime time_active, TTime time_passive, TCount limit = 0);

    virtual void apply(uint8_t value);

    const inline uint8_t get_pin_number() const;
};

template<typename TTime, typename TCount>
PinPulse<TTime, TCount>::PinPulse(uint8_t pin, TTime time_active, TTime time_passive, TCount limit_cycles) :
        Pulse<TTime, TCount>(time_active, time_passive, limit_cycles), pin_number(pin) {}

template<typename TTime, typename TCount>
void PinPulse<TTime, TCount>::apply(uint8_t value) {
#ifdef AVR
    digitalWrite(pin_number, value);
#else
    std::cout << "digitalWrite(" << int(pin_number) << ", " << int(value) << ")" << std::endl;
#endif
}

template<typename TTime, typename TCount>
const inline uint8_t PinPulse<TTime, TCount>::get_pin_number() const {
    return pin_number;
}

} // time

#endif /* PULSE_H_ */
