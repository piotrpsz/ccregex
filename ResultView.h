//
// Created by Piotr Pszczółkowski on 14/03/2024.
//
#pragma once
#include <QTextEdit>

class ResultView : public QTextEdit {
public:
    ResultView(QWidget* = nullptr);
    ~ResultView() override = default;
};
