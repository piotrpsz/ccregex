//
// Created by Piotr Pszczółkowski on 14/03/2024.
//
#pragma once

#include <QTextEdit>

class Editor : public QTextEdit {
public:
    explicit Editor(QWidget* parent = nullptr);
    ~Editor() override = default;
};
