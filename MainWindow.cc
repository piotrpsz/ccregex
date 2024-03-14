//
// Created by Piotr Pszczółkowski on 14/03/2024.
//

/*------- include files:
-------------------------------------------------------------------*/
#include "MainWindow.h"
#include "RegexEdit.h"
#include "ResultView.h"
#include "ReplaceEdit.h"
#include "SourceEdit.h"
#include "OptionsWidget.h"
#include "Settings.h"
#include <QSplitter>

static QString const MainWindowSize = "MainWindow/Size";
static QString const MainWindowPosition = "MainWindow/Position";
static QString const MainWindowState = "MainWindow/State";

MainWindow::MainWindow(QWidget* const parent) :
    QMainWindow(parent),
    main_splitter_{new QSplitter(Qt::Horizontal)},
    windows_splitter_{new QSplitter{Qt::Vertical}},
    replace_edit_{new ReplaceEdit},
    regex_edit_{new RegexEdit},
    source_edit_{new SourceEdit},
    result_view_{new ResultView},
    options_widget_{new OptionsWidget}
{
    setWindowTitle("cc-regex v. 0.0.1");

    windows_splitter_->setHandleWidth(1);
    windows_splitter_->addWidget(regex_edit_);
    windows_splitter_->addWidget(source_edit_);
    windows_splitter_->addWidget(replace_edit_);
    windows_splitter_->addWidget(result_view_);

    main_splitter_->setHandleWidth(1);
    main_splitter_->addWidget(windows_splitter_);
    main_splitter_->addWidget(options_widget_);
    setCentralWidget(main_splitter_);
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
