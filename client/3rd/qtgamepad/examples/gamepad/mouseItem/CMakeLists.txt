# Generated from mouseItem.pro.

cmake_minimum_required(VERSION 3.16)
project(mouseItem VERSION 1.0 LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOMOC ON)

if(NOT DEFINED INSTALL_EXAMPLESDIR)
    set(INSTALL_EXAMPLESDIR "examples")
endif()

set(INSTALL_EXAMPLEDIR "${INSTALL_EXAMPLESDIR}/gamepadlegacy/mouseItem")

find_package(QT NAMES Qt5 Qt6 REQUIRED COMPONENTS Core)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Gui Qml Quick)

qt_add_executable(mouseItem WIN32 MACOSX_BUNDLE
    main.cpp
)
qt_add_qml_module(mouseItem
    URI MouseItem
    VERSION ${PROJECT_VERSION}
    QML_FILES
        qml/main.qml
    NO_RESOURCE_TARGET_PATH
)

target_link_libraries(mouseItem PUBLIC
    Qt::Core
    Qt::Gui
    Qt::Qml
    Qt::Quick
)

install(TARGETS mouseItem
    RUNTIME DESTINATION "${INSTALL_EXAMPLEDIR}"
    BUNDLE DESTINATION "${INSTALL_EXAMPLEDIR}"
    LIBRARY DESTINATION "${INSTALL_EXAMPLEDIR}"
)
