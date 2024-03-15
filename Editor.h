//
// Created by Piotr Pszczółkowski on 14/03/2024.
//
#pragma once

#include "Types.h"
#include <QTextEdit>
#include <vector>
#include <string>

class Editor : public QTextEdit {
public:
    explicit Editor(QWidget* parent = nullptr);
    ~Editor() override = default;

    [[nodiscard]] qstr content() const& {
        return toPlainText();
    }
    void set(std::vector<std::string> const& data) noexcept;
};
