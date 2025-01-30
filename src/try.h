#ifndef TRY_H
#define TRY_H

#include <csetjmp>
#include <option.h>

#define TRY(a) a(_)

#define ERROR_INVALID_OPTION_PEEL 100

struct none_return_buf {
    std::jmp_buf buf;
};

#define DO(type) \
    std::tuple<none_return_buf, Option<type>> _; \
    if (setjmp(std::get<0>(_).buf)) { \
        return std::get<1>(_).unwrap(); \
    } \

#endif