/*
 * avr_mem.h
 */

#ifndef AVR_MEM_H_
#define AVR_MEM_H_

#ifdef __arm__
// should use uinstd.h to define sbrk but Due causes a conflict
extern "C" char* sbrk(int incr);
#else  // __ARM__
extern char *__brkval;
#endif  // __arm__

namespace devtools {

int freeMemory();

} // namespace devtools

#endif /* AVR_MEM_H_ */
