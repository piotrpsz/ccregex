//
// Created by Piotr Pszczółkowski on 14/03/2024.
//
#pragma once

/*------- include files:
-------------------------------------------------------------------*/
#include <QMainWindow>

/*------- forward declarations:
-------------------------------------------------------------------*/
class Component;
class OptionsWidget;
class QSplitter;
class QShowEvent;
class QCloseEvent;

class MainWindow : public QMainWindow{
public:
    explicit MainWindow(QWidget* parent = nullptr);
    ~MainWindow() override = default;

    void showEvent(QShowEvent* event) override;
    void closeEvent(QCloseEvent* event) override;
private:
    QSplitter* const main_splitter_;
    QSplitter* const windows_splitter_;
    Component* const replace_edit_;
    Component* const regex_edit_;
    Component* const source_edit_;
    Component* const result_view_;
    OptionsWidget* const options_widget_;
    bool first_show_{true};
};
