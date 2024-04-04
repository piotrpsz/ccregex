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
// Created by Piotr Pszczółkowski on 26/03/2024.

/*------- include files:
-------------------------------------------------------------------*/
#include "../Types.h"
#include "../Settings.h"
#include "../Content.h"
#include "StdOptionsWidget.h"
#include <QGroupBox>
#include <QCheckBox>
#include <QVBoxLayout>
#include <QRadioButton>
#include <unordered_map>
#include <fmt/core.h>
#include <glaze/glaze.hpp>
using namespace std;

/*------- local constants:
-------------------------------------------------------------------*/
char const *const StdOptionsWidget::EcmaScript = QT_TR_NOOP("ECMA script grammar [default]");
char const *const StdOptionsWidget::BasicPosix = QT_TR_NOOP("basic POSIX grammar");
char const *const StdOptionsWidget::ExtendedPosix = QT_TR_NOOP("extended POSIX grammar");
char const *const StdOptionsWidget::AwkPosix = QT_TR_NOOP("awk utility in POSIX grammar");
char const *const StdOptionsWidget::GrepPosix = QT_TR_NOOP("grep utility in POSIX grammar");
char const *const StdOptionsWidget::GrepPosixE = QT_TR_NOOP("grep with -E in POSIX grammar");
char const *const StdOptionsWidget::IgnoreCase = QT_TR_NOOP("icase [ignore case]");
char const *const StdOptionsWidget::NoSubs = QT_TR_NOOP("nosubs [marked sub-expressions as normal]");
char const *const StdOptionsWidget::Optimize = QT_TR_NOOP("optimize [slower construction, faster matching]");
char const *const StdOptionsWidget::Collate = QT_TR_NOOP("collate [locale sensitive]");
char const *const StdOptionsWidget::Multiline = QT_TR_NOOP("multiline");


StdOptionsWidget::StdOptionsWidget(QWidget* const parent) :
        QWidget(parent),
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
        multiline_{new QCheckBox{tr(Multiline)}}
{
    auto const grammar_group{new QGroupBox{"Grammar option"}};
    auto const grammar_layout{new QVBoxLayout};
    grammar_layout->addWidget(ecma_);
    grammar_layout->addWidget(basic_);
    grammar_layout->addWidget(extended_);
    grammar_layout->addWidget(awk_);
    grammar_layout->addWidget(grep_);
    grammar_layout->addWidget(egrep_);
    grammar_group->setLayout(grammar_layout);

    auto const variation_group{new QGroupBox{"Grammar variation"}};
    auto const variation_layout{new QVBoxLayout};
    variation_layout->addWidget(icace_);
    variation_layout->addWidget(nosubs_);
    variation_layout->addWidget(optimize_);
    variation_layout->addWidget(collate_);
    variation_layout->addWidget(multiline_);
    variation_group->setLayout(variation_layout);

    auto const main_layout{new QVBoxLayout};
    main_layout->addWidget(grammar_group);
    main_layout->addWidget(variation_group);
    main_layout->addStretch();

    main_layout->setSpacing(Settings::NoSpacing);
    main_layout->setContentsMargins(Settings::NoMargins);
    setLayout(main_layout);
}

std::string StdOptionsWidget::options() const noexcept {
    std::unordered_map<std::string, int> data{};
    data["engine"] = int(Engine::Std);

    auto grammar = std::regex_constants::ECMAScript;
    if (basic_->isChecked()) grammar = std::regex_constants::basic;
    else if (extended_->isChecked()) grammar = std::regex_constants::extended;
    else if (awk_->isChecked()) grammar = std::regex_constants::awk;
    else if (grep_->isChecked()) grammar = std::regex_constants::grep;
    else if (egrep_->isChecked()) grammar = std::regex_constants::egrep;
    data["grammar"] = grammar;

    std::regex_constants::syntax_option_type variations{};
    if (icace_->isChecked()) variations |= std::regex_constants::icase;
    if (nosubs_->isChecked()) variations |= std::regex_constants::nosubs;
    if (optimize_->isChecked()) variations |= std::regex_constants::optimize;
    if (collate_->isChecked()) variations |= std::regex_constants::collate;
    if (multiline_->isChecked()) variations |= std::regex_constants::multiline;
    data["variations"] = variations;

//    return glz::prettify(glz::write_json(data));
    return glz::write_json(data);
}

void StdOptionsWidget::update_options(QString const& str) noexcept {
    auto content = Content::from_json(str.toStdString());
    if (auto data = glz::read_json<unordered_map<string, int>>(content->options); data.has_value()) {
        if (auto map = data.value(); map["engine"] == 0) {
            switch (map["engine"]) {
                case std::regex_constants::basic:
                    basic_->setChecked(true);
                    break;
                case std::regex_constants::extended:
                    extended_->setChecked(true);
                    break;
                case std::regex_constants::awk:
                    awk_->setChecked(true);
                    break;
                case std::regex_constants::grep:
                    grep_->setChecked(true);
                    break;
                case std::regex_constants::egrep:
                    egrep_->setChecked(true);
                    break;
                default:
                    ecma_->setChecked(true);
            }
            auto const var = map["variations"];
            if (var & std::regex_constants::icase)
                icace_->setChecked(true);
            if (var & std::regex_constants::nosubs)
                nosubs_->setChecked(true);
            if (var & std::regex_constants::optimize)
                optimize_->setChecked(true);
            if (var & std::regex_constants::collate)
                collate_->setChecked(true);
            if (var & std::regex_constants::multiline)
                multiline_->setChecked(true);
        }
    }
}