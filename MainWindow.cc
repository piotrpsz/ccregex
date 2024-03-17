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
#include "Component.h"
#include "MainWindow.h"
#include "OptionsWidget.h"
#include "EventController.h"
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
qstr const MainWindow::AppName = "cc-regex v. 0.0.1";
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
char const * const MainWindow::NoContentToSave = QT_TR_NOOP("There is no content to save.");
char const * const MainWindow::TryLater = QT_TR_NOOP("If you get something, try again.");
char const * const MainWindow::FileAlreadyExist = QT_TR_NOOP("The file '%1' already exists.");
char const * const MainWindow::WillOverwrite = QT_TR_NOOP("You want to overwrite it?");
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
    replace_edit_{new Component("Replacement String")},
    regex_edit_{new Component("Regular Expression")},
    source_edit_{new Component("Source String")},
    result_view_{new Component("Result", true)},
    options_widget_{new OptionsWidget}
{
    setWindowTitle(AppName);
    create_menu();

    windows_splitter_->setHandleWidth(1);
    windows_splitter_->setChildrenCollapsible(false);
    windows_splitter_->addWidget(regex_edit_);
    windows_splitter_->addWidget(source_edit_);
//    windows_splitter_->addWidget(replace_edit_);
    windows_splitter_->addWidget(result_view_);

    main_splitter_->setHandleWidth(1);
    main_splitter_->setChildrenCollapsible(false);
    main_splitter_->addWidget(windows_splitter_);
    main_splitter_->addWidget(options_widget_);

    setCentralWidget(main_splitter_);

    EventController::instance().append(this, event::RunRequest);
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
    if (auto data = sts.read(LastUsedDirectory); data)
        last_used_dir_ = data.value().toString();
    if (auto data = sts.read(LastUsedFile); data)
        last_used_file_name_ = data.value().toString();
}

void MainWindow::closeEvent(QCloseEvent*) {
    Settings sts;
    sts.save(MainWindowState, saveState());
    sts.save(MainWindowPosition, pos());
    sts.save(MainWindowSize, size());
    sts.save(LastUsedDirectory, last_used_dir_);
    sts.save(LastUsedFile, last_used_file_name_);
}

// Reads data from a file.

void MainWindow::customEvent(QEvent *event) {
    result_view_->clear();
    auto const e = dynamic_cast<Event*>(event);

    switch (int(e->type())) {
        case event::RunRequest: {
            result_view_->clear();
            using namespace std::chrono_literals;
            std::this_thread::sleep_for(500ms);

            auto tool = e->data()[0].toInt();
            auto grammar = e->data()[1].toInt();
            auto variations = e->data()[2].toString();

            if (tool == tool::Std)
                if (auto s = glz::read_json<std::vector<type::StdSyntaxOption>>(variations.toStdString()); s)
                    std_regx(type::StdSyntaxOption(grammar), s.value());
            break;
        }

    }
}


void MainWindow::std_regx(type::StdSyntaxOption grammar, std::vector<type::StdSyntaxOption> vars) const noexcept {
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
}

std::vector<std::string> MainWindow::transform(qstr const& str) noexcept {
    std::vector<std::string> buffer;
    if (not str.isEmpty()) {
        auto data = str.split('\n');
        buffer.reserve(data.size());
        for (auto const item: data) {
            auto text = item.trimmed();
            if (not text.isEmpty())
                buffer.push_back(item.toStdString());
        }
    }
    return std::move(buffer);
}

void MainWindow::open() noexcept {
    clear();

    QFileDialog dialog(this);
    dialog.setOption(QFileDialog::DontUseNativeDialog);
    dialog.setOption(QFileDialog::DontConfirmOverwrite);
    dialog.setFileMode(QFileDialog::AnyFile);
    dialog.setViewMode(QFileDialog::List);
    dialog.setAcceptMode(QFileDialog::AcceptOpen);
    dialog.setNameFilter(tr(NameFilter).arg(FileExt));
    dialog.setDefaultSuffix(FileExt);
    dialog.selectFile(last_used_file_name_);
    dialog.setDirectory(last_used_dir_);
    if (dialog.exec()) {
        auto path = dialog.selectedFiles().first();
        // Remember file references for future use.
        QFileInfo fi{path};
        last_used_dir_ = fi.path();
        last_used_file_name_ = fi.fileName();

        // Read the content.
        std::ifstream f(path.toStdString());
        if (f.is_open()) {
            f.seekg(0, std::ios_base::end);
            auto size = f.tellg();
            f.seekg(0, std::ios_base::beg);
            std::string buffer;
            buffer.resize(size);
            f.read(buffer.data(), size);
            if (f.fail() or f.gcount() not_eq size) {
                std::cerr << tr(ReadError).toStdString() << '\n';
                QMessageBox::critical(this, AppName, tr(ReadError));
                return;
            }
            // Fetch strings from JSON (file content).
            if (auto cnt = Content::from_json(buffer); cnt) {
                auto data = cnt.value();
                regex_edit_->set(data.regex);
                source_edit_->set(data.source);
                replace_edit_->set(data.replacement);
                result_view_->set(data.result);
            }
        }
    }
}

// Saves data to a file.
void MainWindow::save() noexcept {
    auto regex_content = transform(regex_edit_->content().trimmed());
    auto source_content = transform(source_edit_->content().trimmed());
    auto replace_content = transform(replace_edit_->content().trimmed());
    auto result_content = transform(result_view_->content().trimmed());

    Content cnt{regex_content, source_content, replace_content, result_content};
    if (cnt.empty()) {
        QMessageBox box;
        box.setText(tr(NoContentToSave));
        box.setInformativeText(tr(TryLater));
        box.setStandardButtons(QMessageBox::Ok);
        box.exec();
        return;
    }

    QFileDialog dialog(this);
    dialog.setOption(QFileDialog::DontUseNativeDialog);
    dialog.setOption(QFileDialog::DontConfirmOverwrite);
    dialog.setFileMode(QFileDialog::AnyFile);
    dialog.setViewMode(QFileDialog::List);
    dialog.setAcceptMode(QFileDialog::AcceptSave);
    dialog.setNameFilter(tr(NameFilter).arg(FileExt));
    dialog.setDefaultSuffix(FileExt);
    dialog.selectFile(last_used_file_name_);
    dialog.setDirectory(last_used_dir_);
    if (dialog.exec()) {
        auto fi = QFileInfo(dialog.selectedFiles().first());
        if (fi.exists()) {
            QMessageBox box;
            box.setText(QString(tr(FileAlreadyExist)).arg(fi.fileName()));
            box.setInformativeText(tr(WillOverwrite));
            box.setStandardButtons(QMessageBox::Save|QMessageBox::Cancel);
            box.setDefaultButton(QMessageBox::Save);
            if (auto answer = box.exec(); answer == QMessageBox::Cancel)
                return;
        }
        last_used_dir_ = fi.path();
        last_used_file_name_ = fi.fileName();
        std::ofstream f(fi.absoluteFilePath().toStdString());
        if (f.is_open()) {
            auto str = cnt.to_json();
            f.write(str.data(), int(str.size()));
        }
    }
}

void MainWindow::save_as() noexcept {
    qInfo() << "save as slot";
}
