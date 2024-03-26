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

class StdOptionsWidget : public QWidget {
    Q_OBJECT
public:
    StdOptionsWidget(QWidget* = nullptr);
    ~StdOptionsWidget() override = default;

private:
    QRadioButton* const ecma_;
    QRadioButton* const basic_;
    QRadioButton* const extended_;
    QRadioButton* const awk_;
    QRadioButton* const grep_;
    QRadioButton* const egrep_;
    QCheckBox* const icace_;
    QCheckBox* const nosubs_;
    QCheckBox* const optimize_;
    QCheckBox* const collate_;
    QCheckBox* const multiline_;

    static char const * const EcmaScript;
    static char const * const BasicPosix;
    static char const * const ExtendedPosix;
    static char const * const AwkPosix;
    static char const * const GrepPosix;
    static char const * const GrepPosixE;
    static char const * const IgnoreCase;
    static char const * const NoSubs;
    static char const * const Optimize;
    static char const * const Collate;
    static char const * const Multiline;
};
