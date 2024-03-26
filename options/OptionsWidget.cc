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
#include <QBoxLayout>
#include <QPushButton>
#include <QApplication>
#include <QRadioButton>
#include <iostream>
#include <glaze/glaze.hpp>

/*------- local constants:
-------------------------------------------------------------------*/
char const *const OptionsWidget::StdRegex = QT_TR_NOOP("std::regex [standard since C++11]");
char const *const OptionsWidget::QtRegex = QT_TR_NOOP("Qt::QRegularExpression [Qt]");
char const *const OptionsWidget::PcreRegex = QT_TR_NOOP("pcre2 [library using Perl5 syntax and semantic]");
char const *const OptionsWidget::Run = QT_TR_NOOP("Run");
char const *const OptionsWidget::ClearAll = QT_TR_NOOP("Clear");
char const *const OptionsWidget::ClearMatches = QT_TR_NOOP("Clear Matches");
char const *const OptionsWidget::Exit = QT_TR_NOOP("Exit");

/*------- class implementation:
-------------------------------------------------------------------*/
OptionsWidget::OptionsWidget(QWidget *const parent) :
        QWidget(parent),
        options_stacked_{new OptionsStacked},
        std_{new QRadioButton{tr(StdRegex)}},
        qt_{new QRadioButton{tr(QtRegex)}},
        pcre2_{new QRadioButton{tr(PcreRegex)}},
        run_{new QPushButton{tr(Run)}},
        clear_all_{new QPushButton{tr(ClearAll)}},
        clear_matches_{new QPushButton{tr(ClearMatches)}},
        exit_{new QPushButton{tr(Exit)}}
{
    // Initial settings
    std_->setChecked(true);
    pcre2_->setEnabled(false);

    // TOOL group/layout
    auto tool_layout{new QVBoxLayout};
    tool_layout->addWidget(std_);
    tool_layout->addWidget(qt_);
#ifdef PCRE2_REGEX
    tool_layout->addWidget(pcre2_);
#endif
    auto tool_group{new QGroupBox{"Tool"}};
    tool_group->setLayout(tool_layout);
    connect(std_, &QRadioButton::clicked, this, &OptionsWidget::tool_changed);
    connect(qt_, &QRadioButton::clicked, this, &OptionsWidget::tool_changed);

    auto buttons_layout{new QHBoxLayout};
    buttons_layout->addWidget(run_);
    buttons_layout->addWidget(clear_all_);
    buttons_layout->addWidget(clear_matches_);

    auto exit_button_layout{new QHBoxLayout};
    exit_button_layout->addStretch();
    exit_button_layout->addWidget(exit_);

    auto main_layout{new QVBoxLayout};
    main_layout->addWidget(tool_group);
    main_layout->addWidget(options_stacked_);
    main_layout->addStretch();
    main_layout->addLayout(buttons_layout);
    main_layout->addLayout(exit_button_layout);
    setLayout(main_layout);


    auto w = main_layout->minimumSize().width();
    setMinimumWidth(w);
    setMaximumWidth(w);

    connect(run_, &QPushButton::pressed, this, &OptionsWidget::run_slot);
    connect(clear_all_, &QPushButton::pressed, this, &OptionsWidget::claer_all);
    connect(clear_matches_, &QPushButton::pressed, this, &OptionsWidget::claer_matches);
    connect(exit_, &QPushButton::pressed, this, &QApplication::quit);

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
    switch (auto content = options(); content.engine) {
        case Engine::Std: {
            auto json = glz::write_json(content);
            EventController::instance().send_event(event::RunRequest, qstr::fromStdString(json));
            break;
        }
        case Engine::Pcre2: {
            auto json = glz::write_json(content);
            EventController::instance().send_event(event::RunRequest, qstr::fromStdString(json));
            break;
        }
        default: {
        }
    }
}

Content OptionsWidget::options() const noexcept {
//    auto engine = Engine::Std;
//    if (qt_->isChecked()) engine = Engine::Qt;
//    if (pcre2_->isChecked()) engine = Engine::Pcre2;
//
//    type::StdSyntaxOption grammar = std::regex_constants::ECMAScript;
//    if (basic_->isChecked()) grammar = std::regex_constants::basic;
//    if (extended_->isChecked()) grammar = std::regex_constants::extended;
//    if (awk_->isChecked()) grammar = std::regex_constants::awk;
//    if (grep_->isChecked()) grammar = std::regex_constants::grep;
//    if (egrep_->isChecked()) grammar = std::regex_constants::egrep;
//
//    std::vector<type::StdSyntaxOption> variations{};
//    if (icace_->isChecked()) variations.push_back(std::regex_constants::icase);
//    if (nosubs_->isChecked()) variations.push_back(std::regex_constants::nosubs);
//    if (optimize_->isChecked()) variations.push_back(std::regex_constants::optimize);
//    if (collate_->isChecked()) variations.push_back(std::regex_constants::collate);
//    if (multiline_->isChecked()) variations.push_back(std::regex_constants::multiline);
//
//    return Content{.engine = engine, .grammar = grammar, .variations = variations};
    return {};
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
