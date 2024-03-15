//
// Created by Piotr Pszczółkowski on 14/03/2024.
//
#pragma once

#include "Types.h"
#include <QTextEdit>

class Editor : public QTextEdit {
public:
    explicit Editor(QWidget* parent = nullptr);
    ~Editor() override = default;

    qstr content() const noexcept {
        return toPlainText();
    }
};
