cmake_minimum_required(VERSION 3.27)
set(PCRE2 ON)

project(ccregex)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

if (PCRE2)
    add_definitions(-DPCRE2_REGEX)
    message("PCRE2 - The program will be built with the PCRE2-8 library")
else()
    message("No PCRE2 - The program will be built without the PCRE2-8 library")
endif ()

find_package(Qt6 COMPONENTS
        Core
        Gui
        Widgets
        REQUIRED
)
find_package(fmt REQUIRED)

set(APP_SOURCES
        main.cpp
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
        Config.h
)
set(APP_LIBS
        Qt6::Core
        Qt6::Gui
        Qt6::Widgets
        glaze::glaze
        fmt::fmt
)

if (PCRE2)
    set(APP_SOURCES ${APP_SOURCES}
            RegexPcre.cc
            RegexPcre.h
    )
    set(APP_LIBS ${APP_LIBS}
            pcre2-8
    )
endif()

include(FetchContent)
FetchContent_Declare(
            glaze
            GIT_REPOSITORY https://github.com/stephenberry/glaze.git
            GIT_TAG main
            GIT_SHALLOW TRUE
)
FetchContent_MakeAvailable(glaze)

add_executable(ccregex ${APP_SOURCES})
target_link_libraries(ccregex ${APP_LIBS})
