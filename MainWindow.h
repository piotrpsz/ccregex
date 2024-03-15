//
// Created by Piotr Pszczółkowski on 14/03/2024.
//
#pragma once

/*------- include files:
-------------------------------------------------------------------*/
#include "Types.h"
#include <QMainWindow>

/*------- forward declarations:
-------------------------------------------------------------------*/
class QSplitter;
class QShowEvent;
class QCloseEvent;
class OptionsWidget;
class Component;


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
