//
// Created by Piotr Pszczółkowski on 14/03/2024.
//

/*------- include files:
-------------------------------------------------------------------*/
#include "MainWindow.h"
#include "Settings.h"
#include "Content.h"
#include "Component.h"
#include <QDir>
#include <QMenu>
#include <QDebug>
#include <QMenuBar>
#include <QSplitter>
#include <QShortcut>
#include <QFileDialog>
#include "OptionsWidget.h"
#include <vector>
#include <string>

/*------- static constants::
-------------------------------------------------------------------*/
char const * const MainWindow::FileTopMenu = QT_TR_NOOP("File");
char const * const MainWindow::HelpTopMenu = QT_TR_NOOP("Help");
char const * const MainWindow::FileOpen = QT_TR_NOOP("Open File ...");
char const * const MainWindow::FileSave = QT_TR_NOOP("Save File ...");
char const * const MainWindow::FileSaveAs = QT_TR_NOOP("Save File As ...");
char const * const MainWindow::Clear = QT_TR_NOOP("Clear");
qstr const MainWindow::MainWindowSize = "MainWindow/Size";
qstr const MainWindow::MainWindowPosition = "MainWindow/Position";
qstr const MainWindow::MainWindowState = "MainWindow/State";

MainWindow::MainWindow(QWidget* const parent) :
    QMainWindow(parent),
    main_splitter_{new QSplitter(Qt::Horizontal)},
    windows_splitter_{new QSplitter{Qt::Vertical}},
    replace_edit_{new Component("Replacement String")},
    regex_edit_{new Component("Regular Expression")},
    source_edit_{new Component("Source String")},
    result_view_{new Component("Result")},
    options_widget_{new OptionsWidget}
{
    setWindowTitle("cc-regex v. 0.0.1");
    create_menu();

    windows_splitter_->setHandleWidth(1);
    windows_splitter_->setChildrenCollapsible(false);
    windows_splitter_->addWidget(regex_edit_);
    windows_splitter_->addWidget(source_edit_);
    windows_splitter_->addWidget(replace_edit_);
    windows_splitter_->addWidget(result_view_);

    main_splitter_->setHandleWidth(1);
    main_splitter_->setChildrenCollapsible(false);
    main_splitter_->addWidget(windows_splitter_);
    main_splitter_->addWidget(options_widget_);

    setCentralWidget(main_splitter_);
}

void MainWindow::create_menu() noexcept {
    menuBar()->addMenu(new QMenu{tr(HelpTopMenu)});

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

void MainWindow::open() noexcept {
    qInfo() << "open slot";
}

void MainWindow::save() noexcept {
    qInfo() << "save slot";

    auto transform = [](qstr const& str) -> std::vector<std::string> {
        std::vector<std::string> buffer;
        auto data = str.split('\n');
        buffer.reserve(data.size());
        for (auto const item : data)
            buffer.push_back(item.toStdString());
        return std::move(buffer);
    };

    auto regex_content = transform(regex_edit_->content());
    auto source_content = transform(source_edit_->content());
    auto replace_content = transform(replace_edit_->content());
    auto result_content = transform(result_view_->content());

    Content cnt{regex_content, source_content, replace_content, result_content};
    std::cout << cnt << '\n';
    std::cout << "-------------------------\n";
    std::cout << cnt.to_json(true) << '\n';

    auto dir = used_dir_.isEmpty() ? QDir::homePath() : used_dir_;
//    QFileDialog::AcceptSave
    QFileDialog::getSaveFileName(this, "Save data to a file", dir, "", nullptr);
//    dialog.exec();
}

void MainWindow::save_as() noexcept {
    qInfo() << "save as slot";
}

void MainWindow::clear() noexcept {
    qInfo() << "clear slot";
}
