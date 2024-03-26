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
#pragma once

/*------- include files:
-------------------------------------------------------------------*/
#include <QWidget>

/*------- forward declarations:
-------------------------------------------------------------------*/
class QCheckBox;
class QRadioButton;

/*------- class declaration:
-------------------------------------------------------------------*/
class QtOptionsWidget : public QWidget {
    Q_OBJECT
public:
    explicit QtOptionsWidget(QWidget* = nullptr);
    ~QtOptionsWidget() override = default;

private:
    // PATTERN OPTIONS
    QCheckBox* const pattern_case_insensitive_;
    QCheckBox* const pattern_dot_matches_everything_;
    QCheckBox* const pattern_multiline_;
    QCheckBox* const pattern_extended_syntax_;
    QCheckBox* const pattern_inverted_greediness_;
    QCheckBox* const pattern_dont_capture_;
    QCheckBox* const pattern_use_unicode_;
    static char const * const PatternOptionsTitle;
    static char const * const PatternCaseInsensitive;
    static char const * const PatternDotMatchesEverything;
    static char const * const PatternMultiline;
    static char const * const PatternExtendedSyntax;
    static char const * const PatternInvertedGreediness;
    static char const * const PatternDontCapture;
    static char const * const PatternUseUnicode;
    // MATCH TYPES
    QRadioButton* const match_type_normal_;
    QRadioButton* const match_type_partial_prefer_complete_;
    QRadioButton* const match_type_partial_prefer_first_;
    QRadioButton* const match_type_no_match_;
    static char const * const MatchTypesTitle;
    static char const * const MatchTypeNormal;
    static char const * const MatchTypePartialPreferComplete;
    static char const * const MatchTypePartialPreferFirst;
    static char const * const MatchTypeNoMatch;
    // MATCH OPTIONS
    QCheckBox* const match_option_anchored_match_;
    QCheckBox* const match_option_anchor_at_offset_match_;
    QCheckBox* const match_option_dont_check_subject_string_match_;
    static char const * const MatchOptionsTitle;
    static char const * const MatchOptionAnchoredMatch;
    static char const * const MatchOptionAnchorAtOffsetMatch;
    static char const * const MatchOptionDontCheckSubjectStringMatch;
    // WILDCARD CONVERSION OPTIONS
    QCheckBox* const wildcard_conversion_unanchored_;
    QCheckBox* const wildcard_conversion_no_path_;
    static char const * const WildcardConversionOptionsTitle;
    static char const * const WildcardConversionUnanchored;
    static char const * const WildcardConversionNoPath;

};
