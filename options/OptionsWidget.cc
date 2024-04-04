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

/*------- include files:
-------------------------------------------------------------------*/
#include "../Types.h"
#include "OptionsWidget.h"
#include "OptionsStacked.h"
#include "../EventController.h"
#include <QCheckBox>
#include <QGroupBox>
#include <QLabel>
#include <QBoxLayout>
#include <QGridLayout>
#include <QPushButton>
#include <QApplication>
#include <QRadioButton>
#include <iostream>
#include <unordered_map>
#include <glaze/glaze.hpp>

/*------- local constants:
-------------------------------------------------------------------*/
char const *const OptionsWidget::StdRegex = QT_TR_NOOP("std::regex [standard since C++11]");
char const *const OptionsWidget::QtRegex = QT_TR_NOOP("Qt::QRegularExpression [Qt]");
char const *const OptionsWidget::PcreRegex = QT_TR_NOOP("pcre2 [library using Perl5 syntax and semantic]");
char const *const OptionsWidget::NewDocument = QT_TR_NOOP("New");
char const *const OptionsWidget::ClearAll = QT_TR_NOOP("Clear");
char const *const OptionsWidget::ClearMatches = QT_TR_NOOP("Clear Matches");
char const *const OptionsWidget::Run = QT_TR_NOOP("Run");
char const *const OptionsWidget::Exit = QT_TR_NOOP("Exit");

/*------- class implementation:
-------------------------------------------------------------------*/
OptionsWidget::OptionsWidget(QWidget *const parent) :
        QWidget(parent),
        options_stacked_{new OptionsStacked},
        std_{new QRadioButton{tr(StdRegex)}},
        qt_{new QRadioButton{tr(QtRegex)}},
        pcre2_{new QRadioButton{tr(PcreRegex)}}
{
    // Initial settings
    std_->setChecked(true);
    pcre2_->setEnabled(false);

    // TOOL group/layout
    auto const tool_layout{new QVBoxLayout};
    tool_layout->addWidget(std_);
    tool_layout->addWidget(qt_);
#ifdef PCRE2_REGEX
    tool_layout->addWidget(pcre2_);
#endif
    auto const tool_group{new QGroupBox{"Tool"}};
    tool_group->setLayout(tool_layout);
    connect(std_, &QRadioButton::clicked, this, &OptionsWidget::tool_changed);
    connect(qt_, &QRadioButton::clicked, this, &OptionsWidget::tool_changed);

    // Buttons
    auto const run_button{new QPushButton{tr(Run)}};
    auto const exit_button{new QPushButton{tr(Exit)}};
    auto const new_button{new QPushButton{tr(NewDocument)}};
    auto const clear_all_button{new QPushButton{tr(ClearAll)}};
    auto const clear_matches_button{new QPushButton{tr(ClearMatches)}};

    auto const buttons_layout{new QGridLayout};
    buttons_layout->addWidget(run_button, 0, 0);
    buttons_layout->addWidget(exit_button, 0, 2);
    buttons_layout->addWidget(new QLabel, 1, 0);
    buttons_layout->addWidget(new_button, 2, 0);
    buttons_layout->addWidget(clear_all_button, 2, 1);
    buttons_layout->addWidget(clear_matches_button, 2, 2);


    connect(run_button, &QPushButton::pressed, this, &OptionsWidget::run_slot);
    connect(exit_button, &QPushButton::pressed, this, &QApplication::quit);
    connect(clear_all_button, &QPushButton::pressed, this, &OptionsWidget::clear_all);
    connect(clear_matches_button, &QPushButton::pressed, this, &OptionsWidget::clear_matches);
    connect(new_button, &QPushButton::pressed, this, &OptionsWidget::new_document);

    auto main_layout{new QVBoxLayout};
    main_layout->addWidget(tool_group);
    main_layout->addWidget(options_stacked_);
    main_layout->addStretch();
    main_layout->addLayout(buttons_layout);
    setLayout(main_layout);

    // fit to content size and don't resize
    auto w = main_layout->minimumSize().width();
    setMinimumWidth(w);
    setMaximumWidth(w);

    EventController::instance().append(this, event::SaveFileRequest);
}

OptionsWidget::~OptionsWidget() {
    EventController::instance().remove(this);
}

void OptionsWidget::customEvent(QEvent* const event) {
    auto const e = dynamic_cast<Event*>(event);

    switch (auto type = static_cast<int>(e->type()); type) {
        case event::SaveFileRequest: {
            auto content = options();
            auto json = glz::write_json(content);
            EventController::instance().send_event(event::SaveFile, qstr::fromStdString(json));
            return;
        }
        default:
        {}
    }
}

void OptionsWidget::run_slot() const noexcept {
    using namespace std;
    auto content = options();
    auto options = glz::read_json<unordered_map<string,int>>(content.options);
    if (!options) return;

    switch (options.value()["engine"]) {
        case int(Engine::Std): {
            auto json = glz::write_json(content);
            EventController::instance().send_event(event::RunRequest, qstr::fromStdString(json));
            break;
        }
        case int(Engine::Pcre2): {
            auto json = glz::write_json(content);
            EventController::instance().send_event(event::RunRequest, qstr::fromStdString(json));
            break;
        }
        default: {
        }
    }
}

Content OptionsWidget::options() const noexcept {
    auto opts = options_stacked_->options();
    return Content{.options = opts};
}

/********************************************************************
 *                                                                  *
 *                            S L O T S                             *
 *                                                                  *
 ********************************************************************/

// This slot is only one for all radio-buttons from tool group
void OptionsWidget::tool_changed() noexcept {
    if (std_->isChecked()) {
        EventController::instance().send_event(event::StdRegexSelected);
        return;
    }
    if (qt_->isChecked()) {
        EventController::instance().send_event(event::QtRegexSelected);
        return;
    }
    if (pcre2_->isChecked()) {
        EventController::instance().send_event(event::PcreRegexSelected);
        return;
    }
}
