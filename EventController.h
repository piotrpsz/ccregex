//
// Created by Piotr Pszczółkowski on 15/03/2024.
//

#pragma once

/*------- include files:
-------------------------------------------------------------------*/
#include <QObject>
#include <QApplication>
#include <QHash>
#include <QSet>
#include <mutex>
#include "Event.h"
#include "Types.h"

/*------- class declaration:
-------------------------------------------------------------------*/
class EventController : public QObject {
    using EventRegister = qhash<i32, qset<QObject*>>;
    std::mutex mutex_;
    EventRegister data_;
public:
    static EventController& instance() noexcept {
        static EventController ec;
        return ec;
    }
    /// no copy, no move
    EventController(EventController const&) = delete;
    EventController(EventController&&) = delete;
    EventController& operator=(EventController const&) = delete;
    EventController& operator=(EventController&&) = delete;
    ~EventController() override = default;

    /// Registering a subscriber for an event.
    /// \param subscriber subscriber object for registration
    /// \param event_id an event that the subscriber is interested in receiving
    /// \return True if registration was successful, False otherwise.
    bool append(QObject* subscriber, i32 event_id) noexcept;

    /// A subscriber object that opts out of receiving ALL events.
    /// \param subscriber unsubscriber object
    void remove(QObject* subscriber) noexcept;

    /// Sending the event with arguments to all subscribers.
    /// \param event_id event
    /// \param args arguments to send in the event.
    template<typename... T>
    void send_event(i32 event_id, T... args) noexcept {
        std::lock_guard<std::mutex> lock(mutex_);

        if (data_.capacity() not_eq data_.size())
            data_.squeeze();
        if (not data_.contains(event_id))
            return;
        auto const& subscribers = data_[event_id];
        for (auto const receiver : subscribers)
            QApplication::postEvent(receiver, new Event(event_id, args...));
    }

private:
    /// EventsController is Singleton, constructor is private.
    EventController() : QObject() {}

    /// Checks whether the subscriber is already registered for the event.
    /// \param subscriber a subscriber whose presence is checked,
    /// \param event_idthe event the subscriber is interested in
    /// \return True if the subscriber is registered for the indicated event, False otherwise.
    bool exists(QObject* subscriber, i32 event_id) noexcept {
        return data_.contains(event_id) and data_.value(event_id).contains(subscriber);
    }
};
