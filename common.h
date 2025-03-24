/*
 * common.h
 *
 */

#ifndef COMMON_H_
#define COMMON_H_

#include "utils.h"

namespace embd {

class AppBase {
public:
    virtual ~AppBase() = default;

    virtual void init() = 0;
    virtual void loop(const UptimeReference &) = 0;
};

} // namespace embd

#endif /* COMMON_H_ */
