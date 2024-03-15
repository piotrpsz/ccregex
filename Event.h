//
// Created by Piotr Pszczółkowski on 15/03/2024.
//
#pragma once

/*------- include files:
-------------------------------------------------------------------*/
#include <QEvent>
#include "Types.h"

/*------- class:
-------------------------------------------------------------------*/
class Event : public QEvent {
    qvec<qvar> data_;
public:
    template<typename... T>
    explicit Event(int const id, T... args) : QEvent(static_cast<QEvent::Type>(id)) {
        (..., data_.push_back(args));
    }
    qvec<qvar> data() && {
        return std::move(data_);
    }
    qvec<qvar> const& data() const& {
        return data_;
    }
};

/*------- user's events:
-------------------------------------------------------------------*/
namespace event {
    enum {
        Dummy = (QEvent::User + 1),
        Run,
        OpenFile,
        SaveFile,
   };
}
