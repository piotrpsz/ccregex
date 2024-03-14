//
// Created by Piotr Pszczółkowski on 14/03/2024.
//
#pragma once
#include <QTextEdit>

class ReplaceEdit : public QTextEdit {
public:
    ReplaceEdit(QWidget* = nullptr);
    ~ReplaceEdit() override = default;
};
