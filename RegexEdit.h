//
// Created by Piotr Pszczółkowski on 14/03/2024.
//
#pragma once
#include <QTextEdit>

class RegexEdit : public QTextEdit {
public:
    RegexEdit(QWidget* = nullptr);
    ~RegexEdit() override = default;
};
