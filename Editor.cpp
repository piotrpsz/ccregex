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

/*------- include files:
-------------------------------------------------------------------*/
#include "Editor.h"
#include "Highlighter.h"
#include "EventController.h"
#include <fmt/core.h>
using namespace std;

Editor::Editor(Highlighting const highlighting, QWidget* const parent) :
        QTextEdit(parent)
{
    setAcceptRichText(true);
    setLineWrapMode(NoWrap);

    QPalette p = palette();
    p.setColor(QPalette::Base, {0x18, 0x18, 0x18});
    p.setColor(QPalette::Text, {0xe0, 0xe0, 0xe0});
    setPalette(p);

    auto block_fmt = textCursor().blockFormat();
    block_fmt.setLineHeight(5, QTextBlockFormat::LineDistanceHeight);
    textCursor().setBlockFormat(block_fmt);

    QFont font;
    font.setFamily("Menlo");
    font.setKerning(true);
    font.setPointSize(12);
    setFont(font);

    if (highlighting == Highlighting::Yes) {
        highlighter_ = new Highlighter(document());
        EventController::instance().append(this, event::Match);
    }
}

void Editor::customEvent(QEvent *event) {
    auto const e = dynamic_cast<Event*>(event);

    switch (int(e->type())) {
        case event::AppendLine:
            if (isReadOnly()) {
                if (e->data().size() == 1) {
                    auto text = e->data()[0].toString();
                    append(text);
                }
            }
            break;
        case event::Match: {
            auto str = e->data()[0].toString();
            fmt::print("Editor::customEvent::Match: {}\n", str.toStdString());
            break;
        }
        default:
        {}
    }
}

void Editor::set(vector<string> const& data) noexcept {
    clear();
    QString plain_text{};
    for (auto const& text : data )
        plain_text = plain_text.append(qstr::fromStdString(text)).append('\n');
    insertPlainText(plain_text);
}
