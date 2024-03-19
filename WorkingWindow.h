
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
// SOFTWARE.#pragma once
//
// Created by Piotr Pszczółkowski on 18/03/2024.
#pragma once

/*------- include files:
-------------------------------------------------------------------*/
#include "Types.h"
#include "Content.h"
#include "LabeledEditor.h"
#include <QWidget>
#include <vector>
#include <string>

/*------- forward declarations:
-------------------------------------------------------------------*/
class LabeledEditor;
class QSplitter;

/*------- class declaration:
-------------------------------------------------------------------*/
class WorkingWindow : public QWidget {
    Q_OBJECT
public:
    explicit WorkingWindow(qstr path = type::EmptyString, qstr name = type::EmptyString, QWidget* = nullptr);
    ~WorkingWindow() override;

    [[nodiscard]] bool noname() const noexcept { return name_ == type::NoName; }

    /// Insert appropriate content into each editor. \n
    /// The version targets 'noname' and also sets the path and name of the window.
    void set_content(qstr path, Content const& content) noexcept;

    /// Insert appropriate content into each editor.
    void set_content(Content const& content) noexcept;

    /// Return content of all editors.
    [[nodiscard]] Content content() const noexcept {
        auto regex_content = transform(regex_edit_->content().trimmed());
        auto source_content = transform(source_edit_->content().trimmed());
        auto matches_content = transform(matches_view_->content().trimmed());
        return {regex_content, source_content, matches_content};
    }

    /// Delete content in all editors.
    void clear() noexcept {
        regex_edit_->clear();
        source_edit_->clear();
        matches_view_->clear();
    }

    /// Delete content of matches-editor (before run).
    void clear_matches() noexcept {
        matches_view_->clear();
    }

    /// Transform editor's content from one QString to vectors of std-strings.
    static strings transform(qstr const& str) noexcept;

    /// Return path (path works as identifier).
    [[nodiscard]] qstr const& path() const noexcept {
        return path_;
    }
private:
    QSplitter* const splitter_;
    LabeledEditor* const regex_edit_;
    LabeledEditor* const source_edit_;
    LabeledEditor* const matches_view_;

    qstr path_{};
    qstr name_{};
};
