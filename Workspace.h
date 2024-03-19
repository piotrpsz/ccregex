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
    /// Handle user events
    /// \param event - event to handle (firstly cast to user Event).
    void customEvent(QEvent* event) override;

    /// Execute regex process for std.
    /// \param grammar - information about used grammar,
    /// \param variations - other user requirements
    void run_std(type::StdSyntaxOption grammar, std::vector<type::StdSyntaxOption> variations) noexcept;

    /// Open and read file from disk. \n
    /// Content for current mdi-subwindow.
    void open() noexcept;

    /// Save content of current mdi-subwindow to the file on disk. \n
    /// If file already exists will overwrite. \n
    /// If 'noname' call save_as.
    void save() noexcept;

    /// Save content of current mdi-subwindow to file on disk. \n
    /// The user must provide a new file name.
    void save_as() noexcept;

    /// Write content to the file (overwrite if exists).
    /// \param fi - information about the file (path)
    /// \param content - content to save.
    void save(QFileInfo const& fi, const Content& content) noexcept;
    [[nodiscard]] WorkingWindow* current_mdiwidget() const noexcept;

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
