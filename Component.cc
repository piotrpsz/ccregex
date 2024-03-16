// MIT License
//
// Copyright (c) 2023 Piotr Pszczółkowski
// Created by Piotr Pszczółkowski on 14/03/2024.
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

/*------- include files:
-------------------------------------------------------------------*/
#include "Editor.h"
#include "Component.h"
#include <QLabel>
#include <QBoxLayout>

Component::Component(QString const& title, QWidget *parent) :
        QWidget(parent),
        editor_{new Editor}
{
    auto description{new QLabel{title}};
    auto font = description->font();
    font.setPointSize(11);
    description->setFont(font);

    auto header_layout{new QHBoxLayout};
    header_layout->setContentsMargins(4, 4, 0, 4);
    header_layout->setSpacing(0);
    header_layout->addWidget(description);
    header_layout->addStretch();

    auto main_layout{new QVBoxLayout};
    main_layout->setSpacing(0);
    main_layout->setContentsMargins(0, 0, 0, 0);
    main_layout->addLayout(header_layout);
    main_layout->addWidget(editor_);

    setLayout(main_layout);
}
