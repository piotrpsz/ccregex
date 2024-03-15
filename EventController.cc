//
// Created by Piotr Pszczółkowski on 15/03/2024.
//

#include "EventController.h"
#include <mutex>

bool EventController::append(QObject* subscriber, i32 event_id) noexcept {
    std::lock_guard<std::mutex> lock(mutex_);

    if (exists(subscriber, event_id))
        return {};

    data_[event_id] << subscriber;
    return true;
}

void EventController::remove(QObject* subscriber) noexcept {
    std::lock_guard<std::mutex> lock(mutex_);

    if (data_.isEmpty()) return;
    qlist<i32> const ids = data_.keys();
    for (auto it = ids.cbegin(); it < ids.cend(); ++it)
        data_[*it].remove(subscriber);
    data_.squeeze();
}
