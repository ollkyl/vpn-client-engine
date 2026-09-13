# Generated from quickGamepad.pro.

cmake_minimum_required(VERSION 3.16)
project(quickGamepad VERSION 1.0 LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOMOC ON)

if(NOT DEFINED INSTALL_EXAMPLESDIR)
    set(INSTALL_EXAMPLESDIR "examples")
endif()

set(INSTALL_EXAMPLEDIR "${INSTALL_EXAMPLESDIR}/gamepadlegacy/quickGamepad")

find_package(QT NAMES Qt5 Qt6 REQUIRED COMPONENTS Core)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS GamepadLegacy Gui Qml Quick)

qt_add_executable(quickGamepad WIN32 MACOSX_BUNDLE
    main.cpp
)
qt_add_qml_module(quickGamepad
    URI QuickGamepad
    VERSION ${PROJECT_VERSION}
    QML_FILES
        qml/main.qml
        qml/JoystickViewer.qml
        qml/ButtonImage.qml
        qml/LeftThumbstick.qml
        qml/RightThumbstick.qml
        qml/DPad.qml
    RESOURCES
        qml/xboxControllerButtonA.png
        qml/xboxControllerButtonB.png
        qml/xboxControllerButtonX.png
        qml/xboxControllerButtonY.png
        qml/xboxControllerBack.png
        qml/xboxControllerButtonGuide.png
        qml/xboxControllerDPad.png
        qml/xboxControllerLeftShoulder.png
        qml/xboxControllerLeftThumbstick.png
        qml/xboxControllerLeftTrigger.png
        qml/xboxControllerRightShoulder.png
        qml/xboxControllerRightThumbstick.png
        qml/xboxControllerRightTrigger.png
        qml/xboxControllerStart.png
    NO_RESOURCE_TARGET_PATH
)

target_link_libraries(quickGamepad PUBLIC
    Qt::Core
    Qt::GamepadLegacy
    Qt::Gui
    Qt::Qml
    Qt::Quick
)

install(TARGETS quickGamepad
    RUNTIME DESTINATION "${INSTALL_EXAMPLEDIR}"
    BUNDLE DESTINATION "${INSTALL_EXAMPLEDIR}"
    LIBRARY DESTINATION "${INSTALL_EXAMPLEDIR}"
)
