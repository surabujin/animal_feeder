/*
 * dt_adapter.cpp
 *
 */

#include "dt_adapter.h"

namespace utils {
namespace datetime {

int16_t DatimeAccess::add_with_overflow(
		int16_t subject, int16_t argument,
		int16_t lower_limit, int16_t upper_limit) {
	int16_t result  = subject + argument;
	if (result < lower_limit || upper_limit < result) {
		int16_t correction = (upper_limit - lower_limit) + 1;
		if (argument < 0) {
			while (result < lower_limit)
				result += correction;
		} else {
			while (upper_limit < result)
				result -= correction;
		}
	}
	return result;
}

uint16_t DatimeSecondsAccess::read(Datime *subject) {
	return subject->second;
}

void DatimeSecondsAccess::write(Datime *subject, uint16_t value) {
    subject->set(
            subject->year, subject->month, subject->day,
            subject->hour, subject->minute, value);
}

void DatimeSecondsAccess::add(Datime *subject, int16_t value) {
    write(subject, add_with_overflow(subject->second, value, 0, 59));
}


uint16_t DatimeMinutesAccess::read(Datime *subject) {
	return subject->minute;
}

void DatimeMinutesAccess::write(Datime *subject, uint16_t value) {
    subject->set(
            subject->year, subject->month, subject->day,
            subject->hour, value, subject->second);
}

void DatimeMinutesAccess::add(Datime *subject, int16_t value) {
	write(subject, add_with_overflow(subject->minute, value, 0, 59));
}

uint16_t DatimeHoursAccess::read(Datime *subject) {
	return subject->hour;
}

void DatimeHoursAccess::write(Datime *subject, uint16_t value) {
    subject->set(
            subject->year, subject->month, subject->day,
            value, subject->minute, subject->second);
}

void DatimeHoursAccess::add(Datime *subject, int16_t value) {
	write(subject, add_with_overflow(subject->hour, value, 0, 23));
}

uint16_t DatimeDaysAccess::read(Datime *subject) {
	return subject->day;
}

void DatimeDaysAccess::write(Datime *subject, uint16_t value) {
    subject->set(
            subject->year, subject->month, value,
            subject->hour, subject->minute, subject->second);
}

void DatimeDaysAccess::add(Datime *subject, int16_t value) {
	int16_t limit;
	switch(subject->month) {
	case 2: // feb
		limit = 28 + subject->isLeap();
		break;
	case 4: // apr
	case 6: // jun
	case 9: // sep
	case 11: // nov
		limit = 30;
		break;
	default: // jan mar may jul aug oct dec
		limit = 31;
	}
	write(subject, add_with_overflow(subject->day, value, 1, limit));
}

uint16_t DatimeMonthsAccess::read(Datime *subject) {
	return subject->month;
}

void DatimeMonthsAccess::write(Datime *subject, uint16_t value) {
    subject->set(
            subject->year, value, subject->day,
            subject->hour, subject->minute, subject->second);
}

void DatimeMonthsAccess::add(Datime *subject, int16_t value) {
	write(subject, add_with_overflow(subject->month, value, 1, 12));
}

uint16_t DatimeYearsAccess::read(Datime *subject) {
	return subject->year;
}

void DatimeYearsAccess::write(Datime *subject, uint16_t value) {
    subject->set(
            value, subject->month, subject->day,
            subject->hour, subject->minute, subject->second);
}

void DatimeYearsAccess::add(Datime *subject, int16_t value) {
	write(subject, add_with_overflow(subject->year, value, 1900, 2100));
}

} // namespace datetime
} // namespace utils
