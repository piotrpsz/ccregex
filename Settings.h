// MIT License
//
// Copyright (c) 2023 Piotr Pszczółkowski
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
// Created by Piotr Pszczółkowski on 14/03/2024.
#pragma once

/*------- include files:
-------------------------------------------------------------------*/
#include "Types.h"
#include <QString>
#include <QVariant>
#include <QSettings>
#include <optional>

/*------- class:
-------------------------------------------------------------------*/
class Settings : public QSettings {
public:
    Settings() : QSettings() {};
    ~Settings() override = default;
    Settings(Settings const&) = delete;
    Settings& operator=(Settings const&) = delete;

    bool save(qstr const& key, qvar const&& data) noexcept {
        setValue(key, data);
        return NoError == status();
    }

    std::optional<qvar> read(qstr const& key) noexcept {
        if (contains(key))
            if (auto data = value(key); status() == NoError)
                return std::move(data);
        return {};
    }
};
