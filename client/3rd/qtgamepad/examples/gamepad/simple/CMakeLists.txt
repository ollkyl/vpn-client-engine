# Generated from simple.pro.

cmake_minimum_required(VERSION 3.16)
project(simple VERSION 1.0 LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOMOC ON)

if(NOT DEFINED INSTALL_EXAMPLESDIR)
    set(INSTALL_EXAMPLESDIR "examples")
endif()

set(INSTALL_EXAMPLEDIR "${INSTALL_EXAMPLESDIR}/gamepadlegacy/simple")

find_package(QT NAMES Qt5 Qt6 REQUIRED COMPONENTS Core)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS GamepadLegacy Gui)

qt_add_executable(simple WIN32 MACOSX_BUNDLE
    gamepadmonitor.cpp gamepadmonitor.h
    main.cpp
)
target_link_libraries(simple PUBLIC
    Qt::Core
    Qt::GamepadLegacy
    Qt::Gui
)

install(TARGETS simple
    RUNTIME DESTINATION "${INSTALL_EXAMPLEDIR}"
    BUNDLE DESTINATION "${INSTALL_EXAMPLEDIR}"
    LIBRARY DESTINATION "${INSTALL_EXAMPLEDIR}"
)
