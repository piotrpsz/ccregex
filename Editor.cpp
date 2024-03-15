//
// Created by Piotr Pszczółkowski on 14/03/2024.
//

#include "Editor.h"
using namespace std;

Editor::Editor(QWidget* const parent) :
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
}

void Editor::set(vector<string> const& data) noexcept {
    for (auto const& text : data )
        append(QString::fromStdString(text));
}
