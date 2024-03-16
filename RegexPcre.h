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
//
// Created by Piotr Pszczółkowski on 15/03/2024.
// Based on: https://www.pcre.org/current/doc/html/index.html
#pragma once

/*------- include files:
-------------------------------------------------------------------*/
#include "Types.h"
#define PCRE2_CODE_UNIT_WIDTH 8
#include <pcre2.h>
#include <vector>

/*------- include class:
-------------------------------------------------------------------*/
class RegexPcre {
    PCRE2_SPTR8 pattern_;
    PCRE2_SPTR8 subject_;
    PCRE2_SIZE size_;
    pcre2_code* re_;
    pcre2_match_data *match_data_;
public:
    struct Match {
        u32 offset, size;
    };

    RegexPcre(char const* pattern, char const* subject, u32 n);

    /// Searches first match or all if needed.
    /// \param find_all - a flag specifying whether the user wants to find all occurrences matching the pattern
    /// \return vektor of matches occurences (see item_t).
    [[nodiscard]] std::vector<Match> run(bool find_all = false) const noexcept;

private:
    std::vector<Match> rest(PCRE2_SIZE *ovector) const noexcept;
    PCRE2_SIZE update_utf8_size(PCRE2_SIZE ovector) const noexcept;
};
