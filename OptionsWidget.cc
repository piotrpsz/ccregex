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
#include "Types.h"
#include "OptionsWidget.h"
#include "EventController.h"
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
char const *const OptionsWidget::EcmaScript = QT_TR_NOOP("ECMA script grammar [default]");
char const *const OptionsWidget::BasicPosix = QT_TR_NOOP("basic POSIX grammar");
char const *const OptionsWidget::ExtendedPosix = QT_TR_NOOP("extended POSIX grammar");
char const *const OptionsWidget::AwkPosix = QT_TR_NOOP("awk utility in POSIX grammar");
char const *const OptionsWidget::GrepPosix = QT_TR_NOOP("grep utility in POSIX grammar");
char const *const OptionsWidget::GrepPosixE = QT_TR_NOOP("grep with -E in POSIX grammar");
char const *const OptionsWidget::IgnoreCase = QT_TR_NOOP("icase [ignore case]");
char const *const OptionsWidget::NoSubs = QT_TR_NOOP("nosubs [marked sub-expressions as normal]");
char const *const OptionsWidget::Optimize = QT_TR_NOOP("optimize [slower construction, faster matching]");
char const *const OptionsWidget::Collate = QT_TR_NOOP("collate [locale sensitive]");
char const *const OptionsWidget::Multiline = QT_TR_NOOP("multiline");

char const *const OptionsWidget::Run = QT_TR_NOOP("Run");
char const *const OptionsWidget::ClearAll = QT_TR_NOOP("Clear");
char const *const OptionsWidget::ClearMatches = QT_TR_NOOP("Clear Matches");
char const *const OptionsWidget::Exit = QT_TR_NOOP("Exit");

/*------- class implementation:
-------------------------------------------------------------------*/
OptionsWidget::OptionsWidget(QWidget *const parent) :
        QWidget(parent),
        std_{new QRadioButton{tr(StdRegex)}},
        qt_{new QRadioButton{tr(QtRegex)}},
        pcre2_{new QRadioButton{tr(PcreRegex)}},
        ecma_{new QRadioButton{tr(EcmaScript)}},
        basic_{new QRadioButton{tr(BasicPosix)}},
        extended_{new QRadioButton{tr(ExtendedPosix)}},
        awk_{new QRadioButton{tr(AwkPosix)}},
        grep_{new QRadioButton{tr(GrepPosix)}},
        egrep_{new QRadioButton{tr(GrepPosixE)}},
        icace_{new QCheckBox{tr(IgnoreCase)}},
        nosubs_{new QCheckBox{tr(NoSubs)}},
        optimize_{new QCheckBox{tr(Optimize)}},
        collate_{new QCheckBox{tr(Collate)}},
        multiline_{new QCheckBox{tr(Multiline)}},
        run_{new QPushButton{tr(Run)}},
        clear_all_{new QPushButton{tr(ClearAll)}},
        clear_matches_{new QPushButton{tr(ClearMatches)}},
        exit_{new QPushButton{tr(Exit)}}
{
    std_->setChecked(true);
    ecma_->setChecked(true);
    qt_->setEnabled(false);

    auto standard_group{new QGroupBox{"Tool"}};
    auto standard_layout{new QVBoxLayout};
    standard_layout->addWidget(std_);
    standard_layout->addWidget(qt_);
#ifdef PCRE2_REGEX
    standard_layout->addWidget(pcre2_);
#endif
    standard_group->setLayout(standard_layout);

    auto grammar_group{new QGroupBox{"Grammar option"}};
    auto grammar_layout{new QVBoxLayout};
    grammar_layout->addWidget(ecma_);
    grammar_layout->addWidget(basic_);
    grammar_layout->addWidget(extended_);
    grammar_layout->addWidget(awk_);
    grammar_layout->addWidget(grep_);
    grammar_layout->addWidget(egrep_);
    grammar_group->setLayout(grammar_layout);

    auto variation_group{new QGroupBox{"Grammar variation"}};
    auto variation_layout{new QVBoxLayout};
    variation_layout->addWidget(icace_);
    variation_layout->addWidget(nosubs_);
    variation_layout->addWidget(optimize_);
    variation_layout->addWidget(collate_);
    variation_layout->addWidget(multiline_);
    variation_group->setLayout(variation_layout);

    auto buttons_layout{new QHBoxLayout};
    buttons_layout->addWidget(run_);
    buttons_layout->addWidget(clear_all_);
    buttons_layout->addWidget(clear_matches_);

    auto exit_button_layout{new QHBoxLayout};
    exit_button_layout->addStretch();
    exit_button_layout->addWidget(exit_);

    auto main_layout{new QVBoxLayout};
    main_layout->addWidget(standard_group);
    main_layout->addWidget(grammar_group);
    main_layout->addWidget(variation_group);
    main_layout->addStretch(4);
    main_layout->addLayout(buttons_layout);
    main_layout->addStretch(100);
    main_layout->addLayout(exit_button_layout);
    setLayout(main_layout);


    auto w = main_layout->minimumSize().width();
    setMinimumWidth(w);
    setMaximumWidth(w);

    connect(run_, &QPushButton::pressed, this, &OptionsWidget::run_slot);
    connect(clear_all_, &QPushButton::pressed, this, &OptionsWidget::claer_all);
    connect(clear_matches_, &QPushButton::pressed, this, &OptionsWidget::claer_matches);
    connect(exit_, &QPushButton::pressed, this, &QApplication::quit);
}

void OptionsWidget::run_slot() noexcept {
    auto tool = tool::Std;
    if (qt_->isChecked()) tool = tool::Qt;
    if (pcre2_->isChecked()) tool = tool::Pcre2;

    switch (tool) {
        case tool::Std: {
            auto [grammar, variations] = options_std();
            auto json = glz::write_json(variations);
            EventController::instance().send_event(event::RunRequest, tool, grammar, qstr::fromStdString(json));
            break;
        }
        case tool::Pcre2: {
//            auto [grammar, variations] = options_std();
//            auto json = glz::write_json(variations);
            EventController::instance().send_event(event::RunRequest, tool);
            break;
        }
        default: {
        }
    }
}

std::pair<type::StdSyntaxOption, std::vector<type::StdSyntaxOption>>
OptionsWidget::options_std() const noexcept {
    type::StdSyntaxOption grammar = std::regex_constants::ECMAScript;
    if (basic_->isChecked()) grammar = std::regex_constants::basic;
    if (extended_->isChecked()) grammar = std::regex_constants::extended;
    if (awk_->isChecked()) grammar = std::regex_constants::awk;
    if (grep_->isChecked()) grammar = std::regex_constants::grep;
    if (egrep_->isChecked()) grammar = std::regex_constants::egrep;

    std::vector<type::StdSyntaxOption> variations{};
    if (icace_->isChecked()) variations.push_back(std::regex_constants::icase);
    if (nosubs_->isChecked()) variations.push_back(std::regex_constants::nosubs);
    if (optimize_->isChecked()) variations.push_back(std::regex_constants::optimize);
    if (collate_->isChecked()) variations.push_back(std::regex_constants::collate);
    if (multiline_->isChecked()) variations.push_back(std::regex_constants::multiline);

    return {grammar, variations};
}
