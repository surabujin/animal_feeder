/*
 * utils.cpp
 *
 */

#include "utils.h"

UptimeReference::UptimeReference() : loop(0), uptime(0) {}

UptimeReference::UptimeReference(unsigned long millisValue) : loop(0), uptime(millisValue) {}

UptimeReference::UptimeReference(const UptimeReference &source) : loop(source.loop), uptime(source.uptime) {}


void UptimeReference::update(unsigned long millisValue) {
	uint8_t loopAdd = millisValue < uptime ? 1 : 0;
	uptime = millisValue;
	loop += loopAdd;
}

void UptimeReference::add(unsigned long value) {
	add(this, value);
}

void UptimeReference::add(const UptimeReference *reference, unsigned long value) {
	unsigned long origin = reference->uptime;
	uptime = reference->uptime + value;
	loop = reference->loop + (uptime < origin ? 1 : 0);
}

bool UptimeReference::operator <(const UptimeReference &reference) const {
	if (loop != reference.loop)
		return false;
	return uptime < reference.uptime;
}
