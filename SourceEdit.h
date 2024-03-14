//
// Created by Piotr Pszczółkowski on 14/03/2024.
//
#pragma once
#include <QTextEdit>

class SourceEdit : public QTextEdit {
public:
    SourceEdit(QWidget* = nullptr);
    ~SourceEdit() override = default;
};

