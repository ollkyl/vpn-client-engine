# Generated from gamepad.pro.

#####################################################################
## Gamepad Module:
#####################################################################

add_library(GamepadLegacy
    qgamepad.cpp qgamepad.h
    qgamepadbackend.cpp qgamepadbackend_p.h
    qgamepadbackendfactory.cpp qgamepadbackendfactory_p.h
    qgamepadbackendplugin.cpp qgamepadbackendplugin_p.h
    qgamepadkeynavigation.cpp qgamepadkeynavigation.h
    qgamepadmanager.cpp qgamepadmanager.h
    qtgamepadglobal.h
)

# Set module version for Qt module discovery
set_target_properties(GamepadLegacy PROPERTIES
    OUTPUT_NAME GamepadLegacy
    VERSION ${PROJECT_VERSION}
    SOVERSION ${PROJECT_VERSION_MAJOR}
)

target_compile_definitions(GamepadLegacy PRIVATE
    QT_BUILD_GAMEPAD_LIB
    QT_BUILDING_QT
    QT_NO_CAST_TO_ASCII
)

target_link_libraries(GamepadLegacy
    PRIVATE Qt::CorePrivate
    PUBLIC Qt::Core Qt::Gui
)

# Create a Private target alias for compatibility
if(NOT TARGET GamepadLegacyPrivate)
    add_library(GamepadLegacyPrivate INTERFACE)
    target_link_libraries(GamepadLegacyPrivate INTERFACE GamepadLegacy Qt::CorePrivate)
endif()

# Create both Qt:: and Qt6:: namespace aliases immediately after target creation
# so they are available for subdirectories (imports, plugins)
foreach(_qtgamepad_target IN ITEMS GamepadLegacy GamepadLegacyPrivate)
    # Create Qt:: alias
    if(NOT TARGET "Qt::${_qtgamepad_target}")
        if(TARGET "${_qtgamepad_target}")
            add_library("Qt::${_qtgamepad_target}" ALIAS "${_qtgamepad_target}")
        endif()
    endif()
    
    # Create Qt6:: alias
    if(NOT TARGET "Qt6::${_qtgamepad_target}")
        if(TARGET "${_qtgamepad_target}")
            add_library("Qt6::${_qtgamepad_target}" ALIAS "${_qtgamepad_target}")
        endif()
    endif()
endforeach()

# For Android, compile the backend directly into the module (not as separate plugin)
# so JNI functions are in the main app .so without Qt symbol versioning.
if(ANDROID)
    target_sources(GamepadLegacy PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/../plugins/gamepads/android/src/qandroidgamepadbackend.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../plugins/gamepads/android/src/qandroidgamepadbackend_p.h
    )
    target_include_directories(GamepadLegacy PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/../plugins/gamepads/android/src
    )
endif()

# When building this module as a vendored subproject, Qt's header syncing (syncqt)
# infrastructure isn't available, but some sources include headers via the
# installed-style prefix (e.g. <QtGamepadLegacy/qgamepad.h>).
# Provide a minimal generated include tree in the build dir to satisfy those includes.
set(_qtgamepad_include_root "${CMAKE_CURRENT_BINARY_DIR}/include")
set(_qtgamepad_include_dir "${_qtgamepad_include_root}/QtGamepadLegacy")
file(MAKE_DIRECTORY "${_qtgamepad_include_dir}")
file(MAKE_DIRECTORY "${_qtgamepad_include_dir}/private")
set(QTGAMEPADLEGACY_INCLUDE_ROOT "${_qtgamepad_include_root}" CACHE INTERNAL
    "Build-time include root that provides QtGamepadLegacy/<...> headers for vendored qtgamepad")

# Copy public headers
foreach(_hdr IN ITEMS
    qgamepad.h
    qgamepadkeynavigation.h
    qgamepadmanager.h
    qtgamepadglobal.h
)
    configure_file("${CMAKE_CURRENT_SOURCE_DIR}/${_hdr}"
                   "${_qtgamepad_include_dir}/${_hdr}"
                   COPYONLY)
endforeach()

# Copy private headers for Android backend
foreach(_hdr IN ITEMS
    qgamepadbackend_p.h
)
    configure_file("${CMAKE_CURRENT_SOURCE_DIR}/${_hdr}"
                   "${_qtgamepad_include_dir}/private/${_hdr}"
                   COPYONLY)
endforeach()

# Provide the Qt-style CamelCase forwarding headers expected by the sources.
file(WRITE "${_qtgamepad_include_dir}/QGamepadManager"
"#pragma once\n#include <QtGamepadLegacy/qgamepadmanager.h>\n")
file(WRITE "${_qtgamepad_include_dir}/QGamepadKeyNavigation"
"#pragma once\n#include <QtGamepadLegacy/qgamepadkeynavigation.h>\n")
file(WRITE "${_qtgamepad_include_dir}/QGamepad"
"#pragma once\n#include <QtGamepadLegacy/qgamepad.h>\n")

# Provide the header the QML type registration generator looks for:
#   __has_include(<private/qtgamepadquicktypes_p.h>)
file(MAKE_DIRECTORY "${_qtgamepad_include_root}/private")
configure_file("${CMAKE_CURRENT_SOURCE_DIR}/../imports/gamepad/qtgamepadquicktypes_p.h"
               "${_qtgamepad_include_root}/private/qtgamepadquicktypes_p.h"
               COPYONLY)

target_include_directories(GamepadLegacy
    PUBLIC
        "$<BUILD_INTERFACE:${_qtgamepad_include_root}>"
)

if(ANDROID)
    set_property(TARGET GamepadLegacy APPEND PROPERTY QT_ANDROID_BUNDLED_JAR_DEPENDENCIES
        jar/QtAndroidGamepad.jar
    )
    # Android backend is now compiled directly into GamepadLegacy, no separate plugin
    # set_property(TARGET GamepadLegacy APPEND PROPERTY QT_ANDROID_LIB_DEPENDENCIES
    #     plugins/legacygamepads/libplugins_gamepads_androidgamepad.so
    # )
endif()

#### Keys ignored in scope 12:.:.:gamepad.pro:<TRUE>:
# ANDROID_FEATURES = "android.hardware.gamepad"
# qt_internal_add_docs(GamepadLegacy
#     doc/qtgamepad.qdocconf
# )

