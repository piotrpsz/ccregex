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
// Created by Piotr Pszczółkowski on 20/03/2024.

/*------- include files:
-------------------------------------------------------------------*/
#include "Highlighter.h"
#include <QTextDocument>
#include <fmt/core.h>

Highlighter::Highlighter(QTextDocument* const parent) : QSyntaxHighlighter(parent) {
    auto const n = 5;
    auto font = parent->defaultFont();
    for (int i = 0; i < n; ++i) {
        format_[i].setFont(font);
        format_[i].setBackground(QColor{200, 200, 200});
        format_[i].setForeground(QColor{10, 10, 10});
    }
}

void Highlighter::highlightBlock(qstr const& line) {
    if (line.isEmpty()) return;

    fmt::print("Line: {}\n", line.toStdString());
    auto text = line.toStdString();
    for (auto const& match : data_)
        if (text.contains(match.str)) {
            if (match.nr == 0) {
                setFormat(match.pos, match.length, format_[0]);
                fmt::print("-- {}\n", match.as_str());
            }
        }
}
