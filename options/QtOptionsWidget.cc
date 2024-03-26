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
#include "../Settings.h"
#include "QtOptionsWidget.h"
#include <QGroupBox>
#include <QCheckBox>
#include <QVBoxLayout>
#include <QRadioButton>

/*------- local constants:
-------------------------------------------------------------------*/
// pattern options
char const * const QtOptionsWidget::PatternOptionsTitle = "Pattern Options";
char const * const QtOptionsWidget::PatternCaseInsensitive = "CaseInsensitiveOption";
char const * const QtOptionsWidget::PatternDotMatchesEverything = "DotMatchesEverythingOption";
char const * const QtOptionsWidget::PatternMultiline = "MultilineOption";
char const * const QtOptionsWidget::PatternExtendedSyntax = "ExtendedPatternSyntaxOption";
char const * const QtOptionsWidget::PatternInvertedGreediness = "InvertedGreedenessOption";
char const * const QtOptionsWidget::PatternDontCapture = "DontCaptureOption";
char const * const QtOptionsWidget::PatternUseUnicode = "UseUnicodePropertiesOption";
// match types
char const * const QtOptionsWidget::MatchTypesTitle = "Match Types";
char const * const QtOptionsWidget::MatchTypeNormal = "NormalMatch";
char const * const QtOptionsWidget::MatchTypePartialPreferComplete = "PartialPreferCompleteMatch";
char const * const QtOptionsWidget::MatchTypePartialPreferFirst = "PartialPreferFirstMatch";
char const * const QtOptionsWidget::MatchTypeNoMatch = "NoMatch";
// match options
char const * const QtOptionsWidget::MatchOptionsTitle = "Match Options";
char const * const QtOptionsWidget::MatchOptionAnchoredMatch = "AnchoredMatchOption";
char const * const QtOptionsWidget::MatchOptionAnchorAtOffsetMatch = "AnchorAtOffsetMatchOption";
char const * const QtOptionsWidget::MatchOptionDontCheckSubjectStringMatch = "DontCheckSubjectStringMatchOption";
// wildcard conversion options
char const * const QtOptionsWidget::WildcardConversionOptionsTitle = "Wildcard Conversion Options";
char const * const QtOptionsWidget::WildcardConversionUnanchored = "UnanchoredWildcardConversion";
char const * const QtOptionsWidget::WildcardConversionNoPath = "NonPathWildcardConversion";


QtOptionsWidget::QtOptionsWidget(QWidget* const parent) :
        QWidget(parent),
        // pattern options
        pattern_case_insensitive_{new QCheckBox(PatternCaseInsensitive)},
        pattern_dot_matches_everything_{new QCheckBox(PatternDotMatchesEverything)},
        pattern_multiline_{new QCheckBox(PatternMultiline)},
        pattern_extended_syntax_{new QCheckBox(PatternExtendedSyntax)},
        pattern_inverted_greediness_{new QCheckBox(PatternInvertedGreediness)},
        pattern_dont_capture_{new QCheckBox(PatternDontCapture)},
        pattern_use_unicode_{ new QCheckBox(PatternUseUnicode)},
        // match types
        match_type_normal_{new QRadioButton(MatchTypeNormal)},
        match_type_partial_prefer_complete_{new QRadioButton(MatchTypePartialPreferComplete)},
        match_type_partial_prefer_first_{new QRadioButton(MatchTypePartialPreferFirst)},
        match_type_no_match_{new QRadioButton(MatchTypeNoMatch)},
        // match options
        match_option_anchored_match_{new QCheckBox(MatchOptionAnchoredMatch)},
        match_option_anchor_at_offset_match_{new QCheckBox(MatchOptionAnchorAtOffsetMatch)},
        match_option_dont_check_subject_string_match_{new QCheckBox(MatchOptionDontCheckSubjectStringMatch)},
        // wildcard conversion options
        wildcard_conversion_unanchored_{new QCheckBox(WildcardConversionUnanchored)},
        wildcard_conversion_no_path_{new QCheckBox(WildcardConversionNoPath)}
{
    // patterns options
    auto const options_group{new QGroupBox{PatternOptionsTitle}};
    auto const options_layout{new QVBoxLayout};
    options_layout->addWidget(pattern_case_insensitive_);
    options_layout->addWidget(pattern_dot_matches_everything_);
    options_layout->addWidget(pattern_multiline_);
    options_layout->addWidget(pattern_extended_syntax_);
    options_layout->addWidget(pattern_inverted_greediness_);
    options_layout->addWidget(pattern_dont_capture_);
    options_layout->addWidget(pattern_use_unicode_);
    options_group->setLayout(options_layout);
    // match types
    match_type_normal_->setChecked(true);
    auto const match_type_group{new QGroupBox{MatchTypesTitle}};
    auto const match_type_layout{new QVBoxLayout};
    match_type_layout->addWidget(match_type_normal_);
    match_type_layout->addWidget(match_type_partial_prefer_complete_);
    match_type_layout->addWidget(match_type_partial_prefer_first_);
    match_type_layout->addWidget(match_type_no_match_);
    match_type_group->setLayout(match_type_layout);
    // match options
    auto const match_option_group{new QGroupBox(MatchOptionsTitle)};
    auto const match_option_layout{new QVBoxLayout};
    match_option_layout->addWidget(match_option_anchored_match_);
    match_option_layout->addWidget(match_option_anchor_at_offset_match_);
    match_option_layout->addWidget(match_option_dont_check_subject_string_match_);
    match_option_group->setLayout(match_option_layout);
    // wildcard conversion options
    auto const wildcard_conversion_group{new QGroupBox(WildcardConversionOptionsTitle)};
    auto const wildcard_conversion_layout{new QVBoxLayout};
    wildcard_conversion_layout->addWidget(wildcard_conversion_unanchored_);
    wildcard_conversion_layout->addWidget(wildcard_conversion_no_path_);
    wildcard_conversion_group->setLayout(wildcard_conversion_layout);

    auto const main_layout{new QVBoxLayout};
    main_layout->addWidget(options_group);
    main_layout->addWidget(match_type_group);
    main_layout->addWidget(match_option_group);
    main_layout->addWidget(wildcard_conversion_group);
    main_layout->addStretch();

    main_layout->setSpacing(Settings::NoSpacing);
    main_layout->setContentsMargins(Settings::NoMargins);
    setLayout(main_layout);
}
