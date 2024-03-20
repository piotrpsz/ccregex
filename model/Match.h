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
// Created by Piotr Pszczółkowski on 20/03/2024.
#pragma once

/*------- include files:
-------------------------------------------------------------------*/
#include <string>
#include <optional>
#include <fmt/core.h>
#include <glaze/glaze.hpp>

/*------- struct:
-------------------------------------------------------------------*/
struct Match {
    int nr{};
    int pos{};
    int length{};
    std::string str{};

    /// Convert structure to JSON string
    [[nodiscard]] std::string to_json(bool const pretty = false) const noexcept {
        auto buffer = glz::write_json(this);
        if (not pretty)
            return buffer;
        return glz::prettify(buffer);
    }

    /// Create Match-object from JSON string.
    static std::optional<Match> from_json(std::string const& json) noexcept {
        Match match{};
        if (auto const ec = glz::read_json(match, json); ec) {
            fmt::print(stderr, "JSON parser error: {}\n", ec.includer_error);
            return {};
        }
        return match;
    }
};

/*------- template struct for glz:
-------------------------------------------------------------------*/
template<>
struct glz::meta<Match> {
    static constexpr auto value = object(
            "nr", &Match::nr,
            "pos", &Match::pos,
            "length", &Match::length,
            "str", &Match::str
    );
};
