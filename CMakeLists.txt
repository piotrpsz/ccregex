cmake_minimum_required(VERSION 3.27)
project(ccregex)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)


find_package(Qt6 COMPONENTS
        Core
        Gui
        Widgets
        REQUIRED)

add_executable(ccregex main.cpp
        MainWindow.cc
        MainWindow.h
        OptionsWidget.cc
        OptionsWidget.h
        Settings.h
        Editor.cpp
        Editor.h
        Component.cpp
        Component.h)
target_link_libraries(ccregex
        Qt::Core
        Qt::Gui
        Qt::Widgets
)

