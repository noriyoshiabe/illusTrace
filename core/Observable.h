#pragma once

#include <vector>
#include "Observer.h"

namespace illustrace {
namespace core {

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

    void notify(E event) {
        for (auto *observer : observers) {
            observer->notify(static_cast<C *>(this), event);
        }
    }

private:
    std::vector<Observer<C, E> *> observers;
};

} // namespace core
} // namespace illustrace
