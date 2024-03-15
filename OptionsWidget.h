//
// Created by Piotr Pszczółkowski on 14/03/2024.
//
#pragma once
#include <QWidget>

class QCheckBox;
class QPushButton;
class QRadioButton;

class OptionsWidget : public QWidget {
    Q_OBJECT
public:
    explicit OptionsWidget(QWidget* = nullptr);
    ~OptionsWidget() override = default;

private slots:
    void run_slot() noexcept;
    void break_slot() noexcept;

private:
    QRadioButton* const std_;
    QRadioButton* const qt_;
    QRadioButton* const pcre2_;
    QRadioButton* const ecma_script_;
    QRadioButton* const basic_;
    QRadioButton* const extended_;
    QRadioButton* const awk_;
    QRadioButton* const grep_;
    QRadioButton* const egrep_;
    QCheckBox* const icace_;
    QCheckBox* const nosubs_;
    QCheckBox* const optimize_;
    QCheckBox* const collate_;
    QCheckBox* const multiline_;
    QPushButton* const run_;
    QPushButton* const break_;
    QPushButton* const exit_;
};

