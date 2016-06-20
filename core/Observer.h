#pragma once

#include <cstdarg>

namespace illustrace {

template <class C>
class Observer {
public:
    Observer() {};
    virtual ~Observer() {};
    virtual void notify(C *sender, va_list argList) = 0;
};

} // namespace illustrace
