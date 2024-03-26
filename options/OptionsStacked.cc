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
// Created by Piotr Pszczółkowski on 26/03/2024.

/*------- include files:
-------------------------------------------------------------------*/
#include "OptionsStacked.h"
#include "StdOptionsWidget.h"
#include "QtOptionsWidget.h"
#include "PcreOptionsWidget.h"
#include "../EventController.h"

OptionsStacked::OptionsStacked(QWidget* const parent) :
        QStackedWidget(parent),
        std_options_widget_(new StdOptionsWidget),
        qt_options_widget_(new QtOptionsWidget),
        pcre_options_widget_(new PcreOptionsWidget)
{
    addWidget(std_options_widget_);
    addWidget(qt_options_widget_);
    addWidget(pcre_options_widget_);

    EventController::instance().append(this, event::StdRegexSelected);
    EventController::instance().append(this, event::QtRegexSelected);
    EventController::instance().append(this, event::PcreRegexSelected);
}

OptionsStacked::~OptionsStacked() {
    EventController::instance().remove(this);
}

void OptionsStacked::customEvent(QEvent* const event) {
    auto const e = dynamic_cast<Event*>(event);
    switch (auto type = static_cast<int>(e->type()); type) {
        using namespace event;
        case StdRegexSelected:
            setCurrentWidget(std_options_widget_);
            event->accept();
            break;
        case QtRegexSelected:
            setCurrentWidget(qt_options_widget_);
            event->accept();
            break;
        case PcreRegexSelected:
            setCurrentWidget(pcre_options_widget_);
            event->accept();
            break;
        default:
        {}
    }
    QStackedWidget::customEvent(event);
}
