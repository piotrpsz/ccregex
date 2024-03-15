//
// Created by Piotr Pszczółkowski on 14/03/2024.
//

#include "OptionsWidget.h"
#include <QApplication>
#include <QRadioButton>
#include <QCheckBox>
#include <QGroupBox>
#include <QBoxLayout>
#include <QPushButton>
#include <iostream>

OptionsWidget::OptionsWidget(QWidget* const parent) :
    QWidget(parent),
    std_{new QRadioButton{"std::regex (cpp standard)"}},
    qt_{new QRadioButton{"QRegularExpression (Qt standard)"}},
    pcre2_{new QRadioButton{"pcre2 (library using Perl5 syntax and semantic)"}},
    ecma_script_{ new QRadioButton{"ECMAScript modified grammar (default)"}},
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
    ecma_script_->setChecked(true);

    auto standard_group{new QGroupBox{"Tool"}};
    auto standard_layout{new QVBoxLayout};
    standard_layout->addWidget(std_);
    standard_layout->addWidget(qt_);
    standard_layout->addWidget(pcre2_);
    standard_group->setLayout(standard_layout);

    auto grammar_group{new QGroupBox{"Grammar option"}};
    auto grammar_layout{new QVBoxLayout};
    grammar_layout->addWidget(ecma_script_);
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
    std::cout << "run slot\n";
}

void OptionsWidget::break_slot() noexcept {
    std::cout << "break slot\n";
}


