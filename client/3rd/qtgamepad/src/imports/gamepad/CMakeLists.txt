# Generated from gamepad.pro.

#####################################################################
## declarative_gamepad Plugin:
#####################################################################

qt_add_qml_module(GamepadLegacyQuickPrivate
    URI "QtGamepadLegacy"
    VERSION "${PROJECT_VERSION}"
    CLASS_NAME QGamepadModule
    # Ensure the build output layout matches the module URI directory name.
    OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/QtGamepadLegacy"
    SOURCES
        qgamepadmouseitem.cpp qgamepadmouseitem.h
        qtgamepadquicktypes_p.h
    NO_PLUGIN_OPTIONAL
    DEPENDENCIES QtQuick
)

# qt_add_qml_module() does not accept PUBLIC_LIBRARIES/LIBRARIES. Link explicitly.
target_link_libraries(GamepadLegacyQuickPrivate PRIVATE
        Qt::Core
        Qt::Gui
        Qt::Qml
        Qt::Quick
    Qt::GamepadLegacy
)

# Vendored build: provide include path for <QtGamepadLegacy/...> headers.
if(DEFINED QTGAMEPADLEGACY_INCLUDE_ROOT AND NOT QTGAMEPADLEGACY_INCLUDE_ROOT STREQUAL "")
    target_include_directories(GamepadLegacyQuickPrivate PRIVATE "${QTGAMEPADLEGACY_INCLUDE_ROOT}")
endif()

# Ensure the generated <private/qtgamepadquicktypes_p.h> (copied into the build include tree)
# can include "qgamepadmouseitem.h".
target_include_directories(GamepadLegacyQuickPrivate PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}")

#### Keys ignored in scope 13:.:.:gamepad.pro:<TRUE>:
# CXX_MODULE = "gamepad"
# IMPORT_VERSION = "1.$$QT_MINOR_VERSION"
# OTHER_FILES = "qmldir"
# TARGETPATH = "QtGamepad"
