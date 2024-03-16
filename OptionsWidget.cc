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
#include "Config.h"
#include "OptionsWidget.h"
#include "EventController.h"
#include <QCheckBox>
#include <QGroupBox>
#include <QBoxLayout>
#include <QPushButton>
#include <QApplication>
#include <QRadioButton>
#include <iostream>
#include <regex>
#include <glaze/glaze.hpp>


OptionsWidget::OptionsWidget(QWidget* const parent) :
    QWidget(parent),
    std_{new QRadioButton{"std::regex (cpp standard)"}},
    qt_{new QRadioButton{"QRegularExpression (Qt standard)"}},
    pcre2_{new QRadioButton{"pcre2 (library using Perl5 syntax and semantic)"}},
    ecma_{ new QRadioButton{"ECMAScript modified grammar (default)"}},
    basic_{new QRadioButton{"basic POSIX grammar"}},
    extended_{new QRadioButton{"extended POSIX grammar"}},
    awk_{new QRadioButton{"AWK utility in POSIX grammar"}},
    grep_{new QRadioButton{"GREP utility in POSIX grammar"}},
    egrep_{new QRadioButton{"GREP with -E option in POSIX grammar"}},
    icace_{new QCheckBox{"character matching without regard to case."}},
    nosubs_{new QCheckBox{"all marked sub-expressions are treated as non-marking "}},
    optimize_{new QCheckBox{"faster matching, slower construction"}},
    collate_{new QCheckBox{"character ranges locale sensitive"}},
    multiline_{new QCheckBox{"anchor characters for multiline"}},
    run_{new QPushButton{"Run"}},
    break_{new QPushButton{"Break"}},
    exit_{new QPushButton{"Exit"}}
{
    std_->setChecked(true);
    ecma_->setChecked(true);

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
    buttons_layout->addWidget(break_);
    buttons_layout->addWidget(exit_);

    auto main_layout{new QVBoxLayout};
    main_layout->addWidget(standard_group);
    main_layout->addWidget(grammar_group);
    main_layout->addWidget(variation_group);
    main_layout->addStretch(4);
    main_layout->addLayout(buttons_layout);
    main_layout->addStretch(100);
    setLayout(main_layout);

    auto w = main_layout->minimumSize().width();
    setMinimumWidth(w);
    setMaximumWidth(w);

    connect(run_, &QPushButton::pressed, this, &OptionsWidget::run_slot);
    connect(break_, &QPushButton::pressed, this, &OptionsWidget::break_slot);
    connect(exit_, &QPushButton::pressed, this, &QApplication::quit);
}

void OptionsWidget::run_slot() noexcept {
    auto tool = tool::Std;
    if (qt_->isChecked()) tool = tool::Qt;
    if (pcre2_->isChecked()) tool = tool::Pcre2;

    std::regex_constants::syntax_option_type grammar = std::regex_constants::ECMAScript;
    if (basic_->isChecked()) grammar = std::regex_constants::basic;
    if (extended_->isChecked()) grammar = std::regex_constants::extended;
    if (awk_->isChecked()) grammar = std::regex_constants::awk;
    if (grep_->isChecked()) grammar = std::regex_constants::grep;
    if (egrep_->isChecked()) grammar = std::regex_constants::egrep;

    std::vector<std::regex_constants::syntax_option_type> variations{};
    if (icace_->isChecked()) variations.push_back(std::regex_constants::icase);
    if (nosubs_->isChecked()) variations.push_back(std::regex_constants::nosubs);
    if (optimize_->isChecked()) variations.push_back(std::regex_constants::optimize);
    if (collate_->isChecked()) variations.push_back(std::regex_constants::collate);
    if (multiline_->isChecked()) variations.push_back(std::regex_constants::multiline);

    EventController::instance().send_event(event::RunRequest,
                                           tool,
                                           grammar,
                                           qstr::fromStdString(glz::write_json(variations)));
}

void OptionsWidget::break_slot() noexcept {
    EventController::instance().send_event(event::RunRequest);
}


