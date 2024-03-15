//
// Created by Piotr Pszczółkowski on 15/03/2024.
//
#pragma once
#define PCRE2_CODE_UNIT_WIDTH 8
#include <pcre2.h>
#include <vector>

class Regex final {
    PCRE2_SPTR8 pattern_;
    PCRE2_SPTR8 subject_;
    PCRE2_SIZE size_;
    pcre2_code* re_;
    pcre2_match_data *match_data_;
public:
    struct item_t {
        uint32_t offset, size;
    };
public:
    Regex(char const* pattern, char const* subject, uint64_t n);
    [[nodiscard]] std::vector<item_t> run(bool find_all = false) const noexcept;

private:
    std::vector<item_t> rest(PCRE2_SIZE *ovector) const noexcept;
    PCRE2_SIZE update_utf8_size(PCRE2_SIZE ovector) const noexcept;
};
