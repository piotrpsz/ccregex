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
#include "Content.h"
#include "Settings.h"
#include "LabeledEditor.h"
#include "MainWindow.h"
#include "OptionsWidget.h"
#include "EventController.h"
#include "Workspace.h"
#include <QMenu>
#include <QMenuBar>
#include <QSplitter>
#include <QShortcut>
#include <QFileInfo>
#include <QFileDialog>
#include <QMessageBox>
#include <fstream>
#include <vector>
#include <string>
#include <regex>
#include <fmt/core.h>
#include <thread>
#include <chrono>

/*------- static constants::
-------------------------------------------------------------------*/
qstr const MainWindow::AppName = "cc-regex v. 0.1.0";
char const * const MainWindow::FileTopMenu = QT_TR_NOOP("File");
char const * const MainWindow::HelpTopMenu = QT_TR_NOOP("Help");
char const * const MainWindow::FileOpen = QT_TR_NOOP("Open File ...");
char const * const MainWindow::FileSave = QT_TR_NOOP("Save File ...");
char const * const MainWindow::FileSaveAs = QT_TR_NOOP("Save File As ...");
char const * const MainWindow::Clear = QT_TR_NOOP("Clear");
char const * const MainWindow::About = QT_TR_NOOP("About");
char const * const MainWindow::NameFilter = QT_TR_NOOP("RegexPcre: (*.%1)");
char const * const MainWindow::FileExt = QT_TR_NOOP("crgx");
char const * const MainWindow::ReadError = QT_TR_NOOP("Something went wrong while reading the file.");



qstr const MainWindow::MainWindowSize = "MainWindow/Size";
qstr const MainWindow::MainWindowPosition = "MainWindow/Position";
qstr const MainWindow::MainWindowState = "MainWindow/State";
qstr const MainWindow::LastUsedDirectory = "LastUsed/Directory";
qstr const MainWindow::LastUsedFile = "LastUsed/File";
qstr const MainWindow::Error = "Error";

MainWindow::MainWindow(QWidget* const parent) :
    QMainWindow(parent),
    main_splitter_{new QSplitter(Qt::Horizontal)},
    windows_splitter_{new QSplitter{Qt::Vertical}},
    replace_edit_{new LabeledEditor("Replacement String")},
    regex_edit_{new LabeledEditor("Regular Expression")},
    source_edit_{new LabeledEditor("Source String")},
    result_view_{new LabeledEditor("Matches", true)},
    options_widget_{new OptionsWidget},
    workspace_{new Workspace}
{
    auto p = palette();
    p.setColor(QPalette::Window, QColor{60, 60, 60, 255});
    setAutoFillBackground(true);
    setPalette(p);
    setContentsMargins(0, 0, 0, 0);

    setWindowTitle(AppName);
    create_menu();

    main_splitter_->setHandleWidth(0);
    main_splitter_->setChildrenCollapsible(false);
    main_splitter_->addWidget(workspace_);
    main_splitter_->addWidget(options_widget_);
    setCentralWidget(main_splitter_);
}

void MainWindow::create_menu() noexcept {
    auto const file = new QMenu{tr(FileTopMenu)};
    auto const open = new QAction{tr(FileOpen)};
    auto const save = new QAction{tr(FileSave)};
    auto const save_as = new QAction{tr(FileSaveAs)};
    auto const clear = new QAction{tr(Clear)};

    open->setShortcut(QKeySequence(Qt::CTRL | Qt::Key_O));
    save->setShortcut(QKeySequence(Qt::CTRL | Qt::Key_S));
    clear->setShortcut(QKeySequence(Qt::CTRL | Qt::Key_R));

    connect(open, &QAction::triggered, this, &MainWindow::open);
    connect(save, &QAction::triggered, this, &MainWindow::save);
    connect(save_as, &QAction::triggered, this, &MainWindow::save_as);
    connect(clear, &QAction::triggered, this, &MainWindow::clear);

    file->addAction(open);
    file->addAction(save);
    file->addAction(save_as);
    file->addSeparator();
    file->addAction(clear);
    menuBar()->addMenu(file);

    //-------------------------------------

    auto const help = new QMenu{tr(HelpTopMenu)};
    auto const about = new QAction{tr(About)};
    connect(about, &QAction::triggered, this, &MainWindow::about);
    help->addAction(about);
    menuBar()->addMenu(help);
}

void MainWindow::showEvent(QShowEvent*) {
    if (not first_show_) return;
    first_show_ = false;

    auto const s = size();
    int const w0 = static_cast<int>(75. * s.width() / 100.);
    int const w1 = s.width() - w0 - main_splitter_->handleWidth();
    main_splitter_->setSizes({w0, w1});

    Settings sts;
    if (auto data = sts.read(MainWindowSize); data)
        resize(data.value().toSize());
    if (auto data = sts.read(MainWindowPosition); data)
        move(data.value().toPoint());
    if (auto data = sts.read(MainWindowSize); data)
        restoreState(data.value().toByteArray());
}

void MainWindow::closeEvent(QCloseEvent*) {
    Settings sts;
    sts.save(MainWindowState, saveState());
    sts.save(MainWindowPosition, pos());
    sts.save(MainWindowSize, size());
}


void MainWindow::std_regx(type::StdSyntaxOption grammar, std::vector<type::StdSyntaxOption> vars) const noexcept {
    /*
    auto opt = grammar;
    for (auto it : vars)
        opt |= it;

    auto pattern_lines = transform(regex_edit_->content());
    auto source_lines = transform(source_edit_->content());
    if (pattern_lines.empty() or source_lines.empty())
        return;

    for (auto const& pattern : pattern_lines) {
        for (auto const& source : source_lines) {
            try {
                std::regex rgx(pattern, opt);
                auto match_begin_it = std::sregex_iterator(source.begin(), source.end(), rgx);
                auto match_end_it = std::sregex_iterator();
                for (auto it = match_begin_it; it != match_end_it; ++it) {
                    std::smatch match = *it;
                    EventController::instance().send_event(event::AppendLine, "--------------------------");
                    for (int i = 0; i < match.size(); ++i) {
                        auto const str{fmt::format("${}: '{}' ({}, {})", i, match.str(i), match.position(i), match.length(i))};
                        EventController::instance().send_event(event::AppendLine, qstr::fromStdString(str));
                    }
                }
            }
            catch (std::regex_error const& e) {
                auto msg = qstr::fromStdString(e.what());
                QMessageBox::critical((QWidget *) this, Error, msg);
                return;
            }
            EventController::instance().send_event(event::AppendLine, "--- END ---");
        }
    }
     */
}
