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


include(FetchContent)
FetchContent_Declare(
        glaze
        GIT_REPOSITORY https://github.com/stephenberry/glaze.git
        GIT_TAG main
        GIT_SHALLOW TRUE
)
FetchContent_MakeAvailable(glaze)

add_executable(ccregex main.cpp
        MainWindow.cc
        MainWindow.h
        OptionsWidget.cc
        OptionsWidget.h
        Settings.h
        Editor.cpp
        Editor.h
        Component.cc
        Component.h
        EventController.cc
        EventController.h
        Types.h
        Event.h
        Content.h
        RegexPcre.cc
        RegexPcre.h)
target_link_libraries(ccregex
        Qt6::Core
        Qt6::Gui
        Qt6::Widgets
        glaze::glaze
        pcre2-8
)
