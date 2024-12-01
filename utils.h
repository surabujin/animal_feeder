/*
 * utils.h
 *
 */

#ifndef UTILS_H_
#define UTILS_H_

#include <stddef.h>
#include <stdint.h>

class UptimeReference {
public:
	UptimeReference();
	UptimeReference(unsigned long millisValue);
	UptimeReference(const UptimeReference &source);

	void update(unsigned long millisValue);
	void add(unsigned long value);
	void add(const UptimeReference *reference, unsigned long value);
	bool operator < (const UptimeReference &reference) const;
protected:
public:
	uint8_t loop;
	unsigned long uptime;
};

template<typename T>
struct ArrReference {
	T *reference;
	size_t len;

	ArrReference() : reference(nullptr), len(0) {}
	ArrReference(T *ref, size_t l) : reference(ref), len(l) {}
};

#endif /* UTILS_H_ */
