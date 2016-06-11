#pragma once

namespace illustrace {
namespace core {

template <class C, typename E>
class Observer {
public:
    Observer() {};
    virtual ~Observer() {};
    virtual void notify(C *sender, E event) = 0;
};

} // namespace core
} // namespace illustrace
