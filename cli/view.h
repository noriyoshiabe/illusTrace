#pragma once

#include "Observer.h"
#include "Illustrace.h"

namespace illustrace {
namespace cli {

class View : public core::Observer<core::Illustrace, core::IllustraceEvent> {
public:
    View();
    ~View();

    int wait;

private:
    void notify(core::Illustrace *sender, core::IllustraceEvent event, va_list argList);
};

} // namespace cli
} // namespace illustrace
