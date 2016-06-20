#pragma once

#include <vector>
#include "Observer.h"

namespace illustrace {

template <class C>
class Observable {
public:
    void addObserver(Observer<C> *observer) {
        observers.push_back(observer);
    }

    void removeObserver(Observer<C> *observer) {
        auto it = std::find(observers.begin(), observers.end(), observer);
        if (it != observers.end()) {
            observers.erase(it);
        }
    }

    void notify(C *sender, ...) {
        va_list argList;
        for (auto *observer : observers) {
            va_start(argList, sender);
            observer->notify(sender, argList);
            va_end(argList);
        }
    }

private:
    std::vector<Observer<C> *> observers;
};

} // namespace illustrace
