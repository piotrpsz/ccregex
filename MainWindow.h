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
#include <QMainWindow>

/*------- forward declarations:
-------------------------------------------------------------------*/
class Component;
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
    void open() noexcept;
    void save() noexcept;
    void save_as() noexcept;
    void clear() noexcept;
    void about() noexcept;

private:
    QSplitter* const main_splitter_;
    QSplitter* const windows_splitter_;
    Component* const replace_edit_;
    Component* const regex_edit_;
    Component* const source_edit_;
    Component* const result_view_;
    OptionsWidget* const options_widget_;
    bool first_show_{true};
    qstr last_used_dir_{};
    qstr last_used_file_name_{};

    static qstr const AppName;
    static char const * const FileTopMenu;
    static char const * const HelpTopMenu;
    static char const * const FileOpen;
    static char const * const FileSave;
    static char const * const FileSaveAs;
    static char const * const Clear;
    static char const * const About;
    static char const * const NameFilter;
    static char const * const FileExt;
    static char const * const ReadError;
    static char const * const NoContentToSave;
    static char const * const TryLater;
    static char const * const FileAlreadyExist;
    static char const * const WillOverwrite;
    static qstr const MainWindowSize;
    static qstr const MainWindowPosition;
    static qstr const MainWindowState;
    static qstr const LastUsedDirectory;
    static qstr const LastUsedFile;
};
