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

/*------- include files:
-------------------------------------------------------------------*/
#include "Workspace.h"
#include "EventController.h"
#include "WorkingWindow.h"
#include "Settings.h"
#include "OptionsWidget.h"
#include "model/Match.h"
#include <QFileDialog>
#include <QMessageBox>
#include <QMdiSubWindow>
#include <fstream>
#include <fmt/core.h>
#include <glaze/glaze.hpp>

/*------- local constants:
-------------------------------------------------------------------*/
char const *const Workspace::NameFilter = QT_TR_NOOP("Regex: (*.%1)");
char const *const Workspace::FileExt = QT_TR_NOOP("crgx");
char const *const Workspace::ReadError = QT_TR_NOOP("Something went wrong while reading the file.");
char const * const Workspace::NoContentToSave = QT_TR_NOOP("There is no content to save.");
char const * const Workspace::TryLater = QT_TR_NOOP("If you get something, try again.");
char const * const Workspace::FileAlreadyExist = QT_TR_NOOP("The file '%1' already exists.");
char const * const Workspace::WillOverwrite = QT_TR_NOOP("You want to overwrite it?");
qstr const Workspace::LastUsedDirectory = "LastUsed/Directory";
qstr const Workspace::LastUsedFile = "LastUsed/File";
qstr const Workspace::Error = "Error";

/*------- class implementation:
-------------------------------------------------------------------*/
Workspace::Workspace(OptionsWidget* const options_widget, QWidget *const parent) :
    QMdiArea(parent),
    options_widget_{options_widget}
{
    setViewMode(QMdiArea::TabbedView);
    addSubWindow(new WorkingWindow);

    auto p = palette();
    p.setColor(QPalette::Window, Settings::BackgroundColor);
    setAutoFillBackground(true);
    setPalette(p);

    setContentsMargins(Settings::NoMargins);

    // Restore state from previous session.
    Settings sts;
    if (auto data = sts.read(LastUsedDirectory); data)
        last_used_dir_ = data.value().toString();
    if (auto data = sts.read(LastUsedFile); data)
        last_used_file_name_ = data.value().toString();

    // I would like to recive these events.
    EventController::instance().append(this, event::RunRequest);
    EventController::instance().append(this, event::OpenFile);
    EventController::instance().append(this, event::SaveFile);
    EventController::instance().append(this, event::SaveAsFile);
    EventController::instance().append(this, event::ClearAll);
    EventController::instance().append(this, event::ClearMatches);
}

Workspace::~Workspace() {
    Settings sts;
    sts.save(LastUsedDirectory, last_used_dir_);
    sts.save(LastUsedFile, last_used_file_name_);

    // Stop to observe event.
    EventController::instance().remove(this);
}

void Workspace::customEvent(QEvent *event) {
    auto const e = dynamic_cast<Event *>(event);
    auto const type = static_cast<int>(e->type());

    switch (type) {
        case event::RunRequest: {
            // Clear current visible matches content.
            current_mdiwidget()->clear_matches();
            // Fetch user setting.
            auto const data = e->data();
            auto tool = data[0].toInt();
            auto grammar = data[1].toInt();
            auto variations = data[2].toString();
            // Select tool and run.
            if (tool == tool::Std)
                if (auto s = glz::read_json<std::vector<type::StdSyntaxOption>>(variations.toStdString()); s)
                    run_std(type::StdSyntaxOption(grammar), s.value());
            e->accept();
            break;
        }
        case event::OpenFile:
            open();
            e->accept();
            break;
        case event::SaveFile:
            save();
            e->accept();
            break;
        case event::SaveAsFile:
            save_as();
            e->accept();
            break;
        case event::ClearAll:
            current_mdiwidget()->clear();
            e->accept();
            break;
        case event::ClearMatches:
            current_mdiwidget()->clear_matches();
            break;
        default: {}
    }
    QMdiArea::customEvent(event);
}

void Workspace::run_std(type::StdSyntaxOption grammar, std::vector<type::StdSyntaxOption> vars) noexcept {
    auto opt = grammar;
    for (auto it : vars)
        opt |= it;

    auto const content = current_mdiwidget()->content();
    auto pattern_lines = content.regex;
    auto source_lines = content.source;
    // We need and pattern and source text (both).
    if (pattern_lines.empty() or source_lines.empty())
        return;

    for (auto const& pattern : pattern_lines) {
        for (auto const& source : source_lines) {
            try {
                std::regex rgx(pattern, opt);
                auto match_begin_it = std::sregex_iterator(source.begin(), source.end(), rgx);
                auto match_end_it = std::sregex_iterator();
                std::vector<Match> buffer;
                for (auto it = match_begin_it; it != match_end_it; ++it) {
                    std::smatch match = *it;
                    EventController::instance().send_event(event::AppendLine, "--------------------------");
                    for (uint i = 0; i < match.size(); ++i) {
                        auto const pos = int(match.position(i));
                        auto const length = int(match.length(i));
                        auto const str = match.str(i);
                        // TODO: trzeba ujednolicić
                        auto const text{fmt::format("${}: '{}' ({}, {})", i, str, pos, length)};
                        EventController::instance().send_event(event::AppendLine, qstr::fromStdString(text));
                        buffer.push_back(Match{.nr = int(i), .pos = pos, .length = length, .str = str});
                    }
                }
                auto matches_json = glz::write_json(buffer);
                EventController::instance().send_event(event::Match, qstr::fromStdString(matches_json));
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

void Workspace::save() noexcept {
    auto mdi_subwidget = current_mdiwidget();
    if (mdi_subwidget->noname()) {
        save_as();
        return;
    }
    auto const content = mdi_subwidget->content();

    // It is nothing to save.
    if (content.empty()) {
        QMessageBox box;
        box.setText(tr(NoContentToSave));
        box.setInformativeText(tr(TryLater));
        box.setStandardButtons(QMessageBox::Ok);
        box.exec();
        return;
    }

    save(QFileInfo(mdi_subwidget->path()), content);
}

void Workspace::save_as() noexcept {
    auto const mdi_subwidget = current_mdiwidget();
    auto const content = mdi_subwidget->content();

    // It is nothing to save.
    if (content.empty()) {
        QMessageBox box;
        box.setText(tr(NoContentToSave));
        box.setInformativeText(tr(TryLater));
        box.setStandardButtons(QMessageBox::Ok);
        box.exec();
        return;
    }

    QFileDialog dialog(qApp->activeWindow());
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
            box.setStandardButtons(QMessageBox::Save | QMessageBox::Cancel);
            box.setDefaultButton(QMessageBox::Save);
            if (auto answer = box.exec(); answer == QMessageBox::Cancel)
                return;
        }

        save(fi, content);
        mdi_subwidget->set_content(fi.absoluteFilePath(), content);
    }
}

void Workspace::save(QFileInfo const& fi, const Content& content) noexcept {
    last_used_dir_ = fi.absolutePath();
    last_used_file_name_ = fi.fileName();

    std::ofstream f(fi.absoluteFilePath().toStdString());
    if (f.is_open()) {
        auto str = content.to_json();
        f.write(str.data(), int(str.size()));

    }
}

void Workspace::open() noexcept {
    QFileDialog dialog(qApp->activeWindow());
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
        last_used_dir_ = fi.absolutePath();
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
                fmt::print(stderr, "{}\n", tr(ReadError).toStdString());
                QMessageBox::critical(this, Settings::AppName, tr(ReadError));
                return;
            }

            // Fetch strings from JSON (file content).
            auto content = Content::from_json(buffer);
            if (not content)
                return;

            {   // Check if this file is already in a mdi-subwindow
                auto const all_windows = subWindowList();
                for (auto const& mdi_subwindow : all_windows)
                    if (auto mdi_subwidget = dynamic_cast<WorkingWindow*>(mdi_subwindow->widget()); mdi_subwidget)
                        if (mdi_subwidget->path() == fi.absoluteFilePath()) {
                            setActiveSubWindow(mdi_subwindow);
                            mdi_subwidget->set_content(path, content.value());
                            return;
                        }
            }

            // Populate current sub-window if 'noname'.
            if (auto const it = current_mdiwidget(); it->noname()) {
                it->set_content(path, content.value());
                return;
            }

            // New populated sub-window.
            auto ww = new WorkingWindow(fi.absoluteFilePath(), fi.baseName());
            ww->set_content(content.value());
            addSubWindow(ww)->show();
        }
    }
}

WorkingWindow* Workspace::current_mdiwidget() const noexcept {
    if (auto mdi_subwin = currentSubWindow(); mdi_subwin)
        if (auto ww = dynamic_cast<WorkingWindow*>(mdi_subwin->widget()); ww)
            return ww;

    return nullptr;
}