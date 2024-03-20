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
//

/*------- include files:
-------------------------------------------------------------------*/
#include "Settings.h"
#include "WorkingWindow.h"
#include "LabeledEditor.h"
#include <QSplitter>
#include <QHBoxLayout>
#include <QFileInfo>
#include <utility>

/*------- class implementation:
-------------------------------------------------------------------*/
WorkingWindow::WorkingWindow(qstr path, qstr name, QWidget *const parent) :
        QWidget(parent),
        splitter_{new QSplitter{Qt::Vertical}},
        regex_edit_{new LabeledEditor("Regular Expression")},
        source_edit_{new LabeledEditor("Source String", Highlighting::Yes)},
        matches_view_{new LabeledEditor("Matches", Highlighting::No, ReadOnly::Yes)},
        path_{std::move(path)},
        name_{std::move(name)}
{
    auto p = palette();
    p.setColor(QPalette::Base, Settings::BackgroundColor);
    setAutoFillBackground(true);
    setPalette(p);

    splitter_->addWidget(regex_edit_);
    splitter_->addWidget(source_edit_);
    splitter_->addWidget(regex_edit_);
    splitter_->addWidget(matches_view_);
    splitter_->setHandleWidth(Settings::NoHandle);
    splitter_->setContentsMargins(Settings::NoMargins);
    splitter_->setPalette(p);
    splitter_->setAutoFillBackground(true);

    auto box = new QHBoxLayout;
    box->setSpacing(Settings::NoSpacing);
    box->setContentsMargins(Settings::NoMargins);
    box->addWidget(splitter_);
    setLayout(box);

    name_ = path_.isEmpty() ? type::NoName : QFileInfo(path_).baseName();
    setWindowTitle(name_);
}

WorkingWindow::~WorkingWindow() = default;

void WorkingWindow::set_content(qstr path, Content const& content) noexcept {
    path_ = std::move(path);
    name_ = QFileInfo(path_).baseName();
    setWindowTitle(name_);

    source_edit_->set(content.source);
    regex_edit_->set(content.regex);
    matches_view_->set(content.matches);
}

void WorkingWindow::set_content(Content const& content) noexcept {
    source_edit_->set(content.source);
    regex_edit_->set(content.regex);
    matches_view_->set(content.matches);
}

std::vector<std::string> WorkingWindow::transform(qstr const& str) noexcept {
    std::vector<std::string> buffer;
    if (not str.isEmpty()) {
        auto data = str.split('\n');
        buffer.reserve(data.size());
        for (auto const& item: data) {
            auto text = item.trimmed();
            if (not text.isEmpty())
                buffer.push_back(item.toStdString());
        }
    }
    return buffer;
}
