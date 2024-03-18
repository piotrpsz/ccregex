// MIT License
//
// Copyright (c) 2024 Piotr Pszczółkowski
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
// SOFTWARE.
//
// Created by Piotr Pszczółkowski on 18/03/2024.
#pragma once

/*------- include files:
-------------------------------------------------------------------*/
#include "Types.h"
#include "Content.h"
#include <QMdiArea>
#include <QFileInfo>
#include <vector>

/*------- forward declarations:
-------------------------------------------------------------------*/
class QEvent;
class WorkingWindow;

/*------- class:
-------------------------------------------------------------------*/
class Workspace : public QMdiArea {
    Q_OBJECT
public:
    explicit Workspace(QWidget* = nullptr);
    ~Workspace() override;
private:
    void customEvent(QEvent* event) override;
    void open() noexcept;
    void clear() noexcept;
    void save() noexcept;
    void save_as() noexcept;
    void save(QFileInfo const& fi, Content content) noexcept;


    WorkingWindow* current_mdiwidget() const noexcept;

    qstr last_used_dir_{};
    qstr last_used_file_name_{};
    static char const * const NameFilter;
    static char const * const FileExt;
    static char const * const ReadError;
    static char const * const NoContentToSave;
    static char const * const TryLater;
    static char const * const FileAlreadyExist;
    static char const * const WillOverwrite;

    static qstr const LastUsedDirectory;
    static qstr const LastUsedFile;
    static qstr const Error;
};
