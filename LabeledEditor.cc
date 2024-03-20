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
// Created by Piotr Pszczółkowski on 14/03/2024.

/*------- include files:
-------------------------------------------------------------------*/
#include "Editor.h"
#include "Settings.h"
#include "LabeledEditor.h"
#include "EventController.h"
#include <QLabel>
#include <QBoxLayout>

LabeledEditor::LabeledEditor(
        QString const& title,
        Highlighting const highlighting,
        ReadOnly const read_only,
        QWidget* const parent
) :
    QWidget(parent),
    editor_{new Editor(highlighting)}
{
    if (read_only == ReadOnly::Yes) {
        editor_->setReadOnly(true);
        EventController::instance().append(editor_, event::AppendLine);
    }

    auto p = palette();
    p.setColor(QPalette::Base, Settings::BackgroundColor);
    setAutoFillBackground(true);
    setPalette(p);

    auto description{new QLabel{title}};
    auto font = description->font();
    font.setPointSize(11);
    description->setFont(font);

    auto header_layout{new QHBoxLayout};
    header_layout->setContentsMargins(4, 4, 2, 4);
    header_layout->setSpacing(0);
    header_layout->addWidget(description);
    header_layout->addStretch();

    auto main_layout{new QVBoxLayout};
    main_layout->setSpacing(Settings::NoSpacing);
    main_layout->setContentsMargins(Settings::NoMargins);
    main_layout->addLayout(header_layout);
    main_layout->addWidget(editor_);

    setLayout(main_layout);
}
