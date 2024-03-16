// MIT License
//
// Copyright (c) 2024 Piotr Pszczółkowski
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.
//
// Created by Piotr Pszczółkowski on 15/03/2024.

/*------- include files:
-------------------------------------------------------------------*/
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
