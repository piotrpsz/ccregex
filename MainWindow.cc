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
#include "Settings.h"
#include "MainWindow.h"
#include "options/OptionsWidget.h"
#include "Workspace.h"
#include <QMenu>
#include <QMenuBar>
#include <QSplitter>
#include <QShortcut>
#include <string>

/*------- static constants:
-------------------------------------------------------------------*/
char const * const MainWindow::FileTopMenu = QT_TR_NOOP("File");
char const * const MainWindow::HelpTopMenu = QT_TR_NOOP("Help");
char const * const MainWindow::FileOpen = QT_TR_NOOP("Open File ...");
char const * const MainWindow::FileSave = QT_TR_NOOP("Save File ...");
char const * const MainWindow::FileSaveAs = QT_TR_NOOP("Save File As ...");
char const * const MainWindow::Clear = QT_TR_NOOP("Clear");
char const * const MainWindow::About = QT_TR_NOOP("About");
char const * const MainWindow::AboutQt = QT_TR_NOOP("About Qt");
qstr const MainWindow::MainWindowSize = "MainWindow/Size";
qstr const MainWindow::MainWindowPosition = "MainWindow/Position";
qstr const MainWindow::MainWindowState = "MainWindow/State";

/*------- class implementation:
-------------------------------------------------------------------*/
MainWindow::MainWindow(QWidget* const parent) :
    QMainWindow(parent),
    splitter_{new QSplitter(Qt::Horizontal)},
    options_widget_{new OptionsWidget},
    workspace_{new Workspace(options_widget_)}
{
    auto p = palette();
    p.setColor(QPalette::Window, Settings::BackgroundColor);
    setAutoFillBackground(true);
    setPalette(p);
    setContentsMargins(Settings::NoMargins);

    setWindowTitle(Settings::AppName);
    create_menu();

    splitter_->setHandleWidth(0);
    splitter_->setChildrenCollapsible(false);
    splitter_->addWidget(workspace_);
    splitter_->addWidget(options_widget_);
    setCentralWidget(splitter_);
}

void MainWindow::create_menu() noexcept {
    auto const file = new QMenu{tr(FileTopMenu)};
    {
        auto const open = new QAction{tr(FileOpen)};
        open->setShortcut(QKeySequence(Qt::CTRL | Qt::Key_O));
        connect(open, &QAction::triggered, this, &MainWindow::open);
        file->addAction(open);
        auto const save = new QAction{tr(FileSave)};
        save->setShortcut(QKeySequence(Qt::CTRL | Qt::Key_S));
        connect(save, &QAction::triggered, this, &MainWindow::save);
        file->addAction(save);
        auto const save_as = new QAction{tr(FileSaveAs)};
        connect(save_as, &QAction::triggered, this, &MainWindow::save_as);
        file->addAction(save_as);

        file->addSeparator();

        auto const clear = new QAction{tr(Clear)};
        clear->setShortcut(QKeySequence(Qt::CTRL | Qt::Key_R));
        connect(clear, &QAction::triggered, this, &MainWindow::clear);
        file->addAction(clear);
    }
    menuBar()->addMenu(file);
    //-------------------------------------
    auto const help = new QMenu{tr(HelpTopMenu)};
    {
        auto const about = new QAction{tr(About)};
        connect(about, &QAction::triggered, this, &MainWindow::about);
        help->addAction(about);
        auto const about_qt = new QAction(tr(AboutQt));
        connect(about_qt, &QAction::triggered, this, &MainWindow::about_qt);
        help->addAction(about_qt);
    }
    menuBar()->addMenu(help);
}

void MainWindow::showEvent(QShowEvent*) {
    if (not first_show_) return;
    first_show_ = false;

    auto const s = size();
    int const w0 = static_cast<int>(75. * s.width() / 100.);
    int const w1 = s.width() - w0 - splitter_->handleWidth();
    splitter_->setSizes({w0, w1});

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
