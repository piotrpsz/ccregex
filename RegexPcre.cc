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
// Based on: // https://www.pcre.org/current/doc/html/index.html

/*------- include files:
-------------------------------------------------------------------*/
#include "RegexPcre.h"
#include <iostream>

using std::cout;
using std::cerr;

// Creates PcreRegex object.
RegexPcre::RegexPcre(char const *const pattern, char const *const subject, u32 const n) {
    pattern_ = (PCRE2_SPTR8) pattern;
    subject_ = (PCRE2_SPTR8) subject;
    size_ = (PCRE2_SIZE) n;

    int errno;
    PCRE2_SIZE offset;
    if (re_ = pcre2_compile_8(pattern_, PCRE2_ZERO_TERMINATED, PCRE2_UTF, &errno, &offset, nullptr); re_ == nullptr) {
        PCRE2_UCHAR buffer[128];
        pcre2_get_error_message(errno, buffer, sizeof(buffer));
        cerr << "RegexPcre: compilation failed at offset " << offset << ": " << buffer << '\n';
    }
    match_data_ = pcre2_match_data_create_from_pattern_8(re_, nullptr);
}

// Searches first match or all if needed.
std::vector<RegexPcre::Match> RegexPcre::run(bool const find_all) const noexcept {
    auto rc = pcre2_match(re_, subject_, size_, 0, 0, match_data_, nullptr);
    if (rc < 0) {
        if (rc == PCRE2_ERROR_NOMATCH) {
            cout << "RegexPcre: match not found\n";
            return {};
        }
        PCRE2_UCHAR buffer[128];
        pcre2_get_error_message(rc, buffer, sizeof(buffer));
        cerr << "RegexPcre: " << buffer << '\n';
        return {};
    }

    auto ovector = pcre2_get_ovector_pointer(match_data_);

    uint32_t option_bits{};
    pcre2_pattern_info(re_, PCRE2_INFO_ALLOPTIONS, &option_bits);
    auto is_utf8 = (option_bits & PCRE2_UTF) != 0;
    if (is_utf8)
        ovector[1] = update_utf8_size(ovector[1]);

    std::vector<Match> matches{};
    u32 const pos = ovector[0];
    u32 const length = ovector[1] - ovector[0];
    matches.push_back(Match{.offset = pos, .size = length});

    if (not find_all) {
        pcre2_match_data_free(match_data_);
        pcre2_code_free(re_);
        matches.shrink_to_fit();
        return matches;
    }

    auto rest_matches = rest(ovector);
    if (not rest_matches.empty())
        std::copy(rest_matches.cbegin(), rest_matches.cend(), std::back_inserter(matches));

    pcre2_match_data_free(match_data_);
    pcre2_code_free(re_);
    matches.shrink_to_fit();
    return matches;
}

// Searches all next matches.
std::vector<RegexPcre::Match> RegexPcre::rest(PCRE2_SIZE *ovector) const noexcept {
    u32 option_bits{};
    pcre2_pattern_info(re_, PCRE2_INFO_ALLOPTIONS, &option_bits);
    auto is_utf8 = (option_bits & PCRE2_UTF) not_eq 0;

    u32 newline{};
    pcre2_pattern_info(re_, PCRE2_INFO_NEWLINE, &newline);
    bool is_crlf_newline =
            newline == PCRE2_NEWLINE_ANY or
            newline == PCRE2_NEWLINE_CRLF or
            newline == PCRE2_NEWLINE_ANYCRLF;

    std::vector<Match> matches{};

    for (;;) {
        u32 options{};
        auto start_offset = ovector[1];

        if (ovector[0] == ovector[1]) {
            if (ovector[0] == size_)
                break;
            options = PCRE2_NOTEMPTY_ATSTART | PCRE2_ANCHORED;
        }
        else {
            PCRE2_SIZE startchar = pcre2_get_startchar(match_data_);
            if (start_offset <= startchar) {
                if (startchar >= size_)
                    break;
                start_offset = startchar + 1;
                if (is_utf8)
                    for ( ; start_offset < size_; start_offset++)
                        if ((subject_[start_offset] & 0xc0) != 0x80)
                            break;
            }
        }

        // Run the next matching operation
        auto rc = pcre2_match(re_, subject_, size_, start_offset, options, match_data_, nullptr);
        if (rc == PCRE2_ERROR_NOMATCH) {
            if (options == 0)
                break;
            ovector[1] = start_offset + 1;
            if (is_crlf_newline and start_offset < size_ - 1 and subject_[start_offset == '\r' and subject_[start_offset + 1] == '\n'])
                ovector[1] += 1;
            else if (is_utf8) {
                while (ovector[1] < size_) {
                    if ((subject_[ovector[1]] & 0xc0) not_eq 0x80)
                        break;
                    ovector[1] += 1;
                }
            }
            continue;
        }

        if (rc < 0)
            return {};

        if (is_utf8)
            ovector[1] = update_utf8_size(ovector[1]);

        u32 const pos = ovector[0];
        u32 const length = ovector[1] - ovector[0];
        matches.push_back(Match{.offset =  pos, .size =  length});
    }
    matches.shrink_to_fit();
    return matches;
}

PCRE2_SIZE RegexPcre::update_utf8_size(PCRE2_SIZE ovector) const noexcept {
    u8 const c = subject_[ovector];

    int n{};
    if (c == 0xc5 or c == 0xc4 or c == 0xc3) n = 2;

    if (n not_eq  0)
        while (ovector < size_ and n > 0) {
            ++ovector;
            --n;
        }

    return ovector;
}
