/*
 * periodic.h
 *
 */

#ifndef PERIODIC_H_
#define PERIODIC_H_

#include "utils.h"

namespace lag {

template<typename T>
class Periodic {
protected:
    embd::UptimeReference period_end_at;

	T period_time_ms;

public:
	Periodic(T time_period) : period_time_ms(time_period) {}
	virtual ~Periodic() = default;

    bool loop(const embd::UptimeReference &uptime) {
		if (uptime < period_end_at)
			return false;

		period_end_at.add(&uptime, period_time_ms);
		action();
		return true;
	}

	virtual void action() = 0;

    inline const embd::UptimeReference* get_period_end() const {
		return &period_end_at;
	}
};

} // namespace time

#endif /* PERIODIC_H_ */
