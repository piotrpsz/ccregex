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
// Created by Piotr Pszczółkowski on 15/03/2024.
#pragma once

/*------- include files:
-------------------------------------------------------------------*/
#include <QVariant>
#include <QString>
#include <QSet>
#include <QList>
#include <regex>
#include <vector>
#include <string>


namespace tool {
    enum {
        Std = 0,
        Pcre2,
        Qt
    };


}

/*------- types:
-------------------------------------------------------------------*/
using i8 = qint8;
using u8 = quint8;
using i32 = qint32;
using u32 = quint32;
using u64 = quint64;
using isize = qsizetype;
using qstr = QString;
using qvar = QVariant;
using strings = std::vector<std::string>;


/*------- template types:
-------------------------------------------------------------------*/
template<typename T>
    using qvec = QVector<T>;
template<typename T>
    using qset = QSet<T>;
template<typename K, typename V>
    using qhash = QHash<K,V>;
template<typename T>
    using qlist = QList<T>;

namespace type {
    using StdSyntaxOption = std::regex_constants::syntax_option_type;
    static inline qstr const EmptyString{};
    static inline qstr const NoName{"noname"};
}

enum class Highlighting {
    No,
    Yes
};
enum class ReadOnly {
    No,
    Yes,
};

