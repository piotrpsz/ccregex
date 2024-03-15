//
// Created by Piotr Pszczółkowski on 15/03/2024.
//
#pragma once

#include "Types.h"
#include <iostream>
#include <string>
#include <vector>
#include <optional>
#include <glaze/glaze.hpp>

struct Content {
    std::vector<std::string> regex{};
    std::vector<std::string> source{};
    std::vector<std::string> replacement{};
    std::vector<std::string> result{};

    Content() = default;
    Content(auto regex_, auto source_, auto replacement_, auto result_) :
        regex{std::move(regex_)},
        source{std::move(source_)},
        replacement{std::move(replacement_)},
        result{std::move(result_)}
    {}
    ~Content() = default;
    Content(Content const&) = default;
    Content(Content&&) = default;
    Content& operator=(Content const&) = default;
    Content& operator=(Content&&) = default;

    [[nodiscard]] bool empty() const noexcept {
        return regex.empty() and source.empty() and replacement.empty() and result.empty();
    }

    [[nodiscard]] std::string to_json(bool const pretty = false) const noexcept {
        auto buffer = glz::write_json(this);
        if (not pretty)
            return buffer;
        return glz::prettify(buffer);
    }

    static std::optional<Content> from_json(std::string const& json) noexcept {
        Content cnt;
        if (auto const ec = glz::read_json(cnt, json); ec) {
            std::cerr << "JSON parse error: " << ec.includer_error << '\n';
            return {};
        }
        return std::move(cnt);
    }

    friend std::ostream &operator<<(std::ostream &s, Content const &r) noexcept {
        s << "-- Regex:\n";
        for (auto const& str : r.regex)
            s << '\t' << str << '\n';
        s << "-- Source:\n";
        for (auto const& str : r.source)
            s << '\t' << str << '\n';
        s << "-- Replacement:\n";
        for (auto const& str : r.replacement)
            s << '\t' << str << '\n';
        s << "-- Result:\n";
        for (auto const& str : r.result)
            s << '\t' << str << '\n';

        return s;
    }
};

template<>
struct glz::meta<Content> {
    static constexpr auto value = object(
            "regex", &Content::regex,
            "source", &Content::source,
//            "replacement", &Content::replacement,
            "result", &Content::result
            );
};
