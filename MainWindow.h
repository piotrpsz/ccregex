// MIT License
//
// Copyright (c) 2023 Piotr Pszczółkowski
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.#pragma once
//
// Created by Piotr Pszczółkowski on 14/03/2024.

/*------- include files:
-------------------------------------------------------------------*/
#include "Types.h"
#include "LabeledEditor.h"
#include "EventController.h"
#include <QMainWindow>
#include <QMessageBox>

/*------- forward declarations:
-------------------------------------------------------------------*/
class Workspace;
class LabeledEditor;
class QSplitter;
class QShowEvent;
class QCloseEvent;
class OptionsWidget;

/*------- class:
-------------------------------------------------------------------*/
class MainWindow : public QMainWindow{
    Q_OBJECT
public:
    explicit MainWindow(QWidget* parent = nullptr);
    ~MainWindow() override = default;

    void create_menu() noexcept;
    void showEvent(QShowEvent* event) override;
    void closeEvent(QCloseEvent* event) override;

private slots:
    static void open() noexcept {
        EventController::instance().send_event(event::OpenFile);
    };
    static void clear() noexcept {
        EventController::instance().send_event(event::ClearAll);
    }
    void save() noexcept {
        EventController::instance().send_event(event::SaveFile);
    }
    void save_as() noexcept {
        EventController::instance().send_event(event::SaveAsFile);
    }

    void about() noexcept {
        QMessageBox::about(this, "About",
                           "cc-regex is a regular expression testing program.\n"
                           "The program uses tools available for C++ programmers.\n\n"
                           "Author: Piotr Pszczółkowski (piotr@beesoft.pl)."
        );
    }

private:
    QSplitter* const splitter_;
    OptionsWidget* const options_widget_;
    Workspace* const workspace_;
    bool first_show_{true};

    static char const * const FileTopMenu;
    static char const * const HelpTopMenu;
    static char const * const FileOpen;
    static char const * const FileSave;
    static char const * const FileSaveAs;
    static char const * const Clear;
    static char const * const About;
    static qstr const MainWindowSize;
    static qstr const MainWindowPosition;
    static qstr const MainWindowState;
};
