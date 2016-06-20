#pragma once

#include <vector>
#include "Observer.h"

namespace illustrace {
namespace lib {

template <class C, typename E>
class Observable {
public:
    void addObserver(Observer<C, E> *observer) {
        observers.push_back(observer);
    }

    void removeObserver(Observer<C, E> *observer) {
        auto it = std::find(observers.begin(), observers.end(), observer);
        if (it != observers.end()) {
            observers.erase(it);
        }
    }

    void notify(E event, ...) {
        va_list argList;
        for (auto *observer : observers) {
            va_start(argList, event);
            observer->notify(static_cast<C *>(this), event, argList);
            va_end(argList);
        }
    }

private:
    std::vector<Observer<C, E> *> observers;
};

} // namespace lib
} // namespace illustrace
