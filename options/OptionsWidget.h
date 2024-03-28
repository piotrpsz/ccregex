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
#include "../EventController.h"
#include "../Content.h"
#include <QWidget>
#include <utility>

/*------- forward declarations:
-------------------------------------------------------------------*/
class QEvent;
class QCheckBox;
class QPushButton;
class QRadioButton;
class OptionsStacked;

/*------- class declaration:
-------------------------------------------------------------------*/
class OptionsWidget : public QWidget {
    Q_OBJECT
public:
    explicit OptionsWidget(QWidget* = nullptr);
    ~OptionsWidget() override;

    [[nodiscard]] Content options() const noexcept;
    void customEvent(QEvent* event) override;

private slots:
    void run_slot() const noexcept;
    static void clear_all() noexcept {
        EventController::instance().send_event(event::ClearAll);
    }
    static void clear_matches() noexcept {
        EventController::instance().send_event(event::ClearMatches);
    }
    static void new_document() noexcept {
        EventController::instance().send_event(event::New);
    }
    void tool_changed() noexcept;

private:
    OptionsStacked* const options_stacked_;
    QRadioButton* const std_;
    QRadioButton* const qt_;
    QRadioButton* const pcre2_;

    static char const * const StdRegex;
    static char const * const QtRegex;
    static char const * const PcreRegex;

    static char const * const NewDocument;
    static char const * const ClearAll;
    static char const * const ClearMatches;
    static char const * const Run;
    static char const * const Exit;
};
