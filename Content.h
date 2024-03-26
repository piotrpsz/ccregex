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
// Created by Piotr Pszczółkowski on 15/03/2024.
#pragma once

/*------- include files:
-------------------------------------------------------------------*/
#include "Types.h"
#include <iostream>
#include <string>
#include <vector>
#include <optional>
#include <glaze/glaze.hpp>
#include <fmt/core.h>

/*------- struct:
-------------------------------------------------------------------*/
struct Content {
    Engine engine{Engine::Std};
    strings source{};
    strings regex{};
    strings matches{};
    type::StdSyntaxOption grammar{};
    std::vector<type::StdSyntaxOption> variations{};

    bool empty() const noexcept {
        return source.empty() and regex.empty();
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
        s << "-- RegexPcre:\n";
        for (auto const& str : r.regex)
            s << '\t' << str << '\n';
        s << "-- Source:\n";
        for (auto const& str : r.source)
            s << '\t' << str << '\n';
        s << "-- Result:\n";
        for (auto const& str : r.matches)
            s << '\t' << str << '\n';
        s << "-- Grammar: " << r.grammar << '\n';
        s << "-- Variationd: \n";
        for (auto const& item : r.variations)
            s << '\t' << item << '\n';

        return s;
    }
};

/*------- template struct for glz:
-------------------------------------------------------------------*/
template<>
struct glz::meta<Content> {
    static constexpr auto value = object(
            "engine", &Content::engine,
            "regex", &Content::regex,
            "source", &Content::source,
            "result", &Content::matches,
            "grammar", &Content::grammar,
            "variations", &Content::variations
    );
};
