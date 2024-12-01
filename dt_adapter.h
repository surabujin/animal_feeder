/*
 * dt_adapter.h
 *
 */

#ifndef DT_ADAPTER_H_
#define DT_ADAPTER_H_

#include <stdint.h>

#include <core/Datime.h>

namespace utils {
namespace datetime {

class DatimeAccess {
public:
	virtual ~DatimeAccess() = default;

	virtual uint16_t read(Datime *subject) = 0;
	virtual void write(Datime *subject, uint16_t value) = 0;

	virtual void add(Datime *subject, int16_t value) = 0;

protected:
	static int16_t add_with_overflow(int16_t subject, int16_t argument, int16_t lower_limit, int16_t upper_limit);
};

class DatimeSecondsAccess : public DatimeAccess {
public:
	virtual uint16_t read(Datime *subject);
	virtual void write(Datime *subject, uint16_t value);

	virtual void add(Datime *subject, int16_t value);
};

class DatimeMinutesAccess : public DatimeAccess {
public:
	virtual uint16_t read(Datime *subject);
	virtual void write(Datime *subject, uint16_t value);

	virtual void add(Datime *subject, int16_t value);
};

class DatimeHoursAccess : public DatimeAccess {
public:
	virtual uint16_t read(Datime *subject);
	virtual void write(Datime *subject, uint16_t value);

	virtual void add(Datime *subject, int16_t value);
};

class DatimeDaysAccess : public DatimeAccess {
public:
	virtual uint16_t read(Datime *subject);
	virtual void write(Datime *subject, uint16_t value);

	virtual void add(Datime *subject, int16_t value);
};

class DatimeMonthsAccess : public DatimeAccess {
public:
	virtual uint16_t read(Datime *subject);
	virtual void write(Datime *subject, uint16_t value);

	virtual void add(Datime *subject, int16_t value);
};

class DatimeYearsAccess : public DatimeAccess {
public:
	virtual uint16_t read(Datime *subject);
	virtual void write(Datime *subject, uint16_t value);

	virtual void add(Datime *subject, int16_t value);
};

} // namespace datetime
} // namespace utils

#endif /* DT_ADAPTER_H_ */
