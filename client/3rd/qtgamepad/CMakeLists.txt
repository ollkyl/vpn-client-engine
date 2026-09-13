# Generated from qtgamepad.pro.

cmake_minimum_required(VERSION 3.16)

# When this repository is built standalone (like an official Qt module checkout),
# `.cmake.conf` sets `QT_REPO_MODULE_VERSION`. When the same sources are vendored
# via `add_subdirectory()` into a user project, having `QT_REPO_MODULE_VERSION`
# set makes Qt think it's "building Qt", which triggers global promotion of
# imported 3rd party targets and can fail with modern CMake versions.
#
# So: only include `.cmake.conf` for standalone builds.
set(_qtgamepad_is_standalone FALSE)
if(CMAKE_SOURCE_DIR STREQUAL CMAKE_CURRENT_SOURCE_DIR)
    set(_qtgamepad_is_standalone TRUE)
endif()

set(_qtgamepad_project_version "0.0.0")
if(_qtgamepad_is_standalone)
include(.cmake.conf)
    set(_qtgamepad_project_version "${QT_REPO_MODULE_VERSION}")
else()
    unset(QT_REPO_MODULE_VERSION)
    # The Qt CMake helpers create the versioned target aliases as Qt${PROJECT_VERSION_MAJOR}::Foo.
    # When vendored as a subproject, we still want the usual Qt6:: targets, otherwise Qt's plugin
    # helpers won't be able to associate plugins with this module.
    set(_qtgamepad_project_version "6.0.0")
endif()

project(QtGamepadLegacy
    VERSION "${_qtgamepad_project_version}"
    DESCRIPTION "Qt Gamepad (legacy) Libraries"
    HOMEPAGE_URL "https://qt.io/"
    LANGUAGES CXX C
)

if(_qtgamepad_is_standalone)
find_package(Qt6 ${PROJECT_VERSION} CONFIG REQUIRED COMPONENTS BuildInternals Core Gui)
find_package(Qt6 ${PROJECT_VERSION} CONFIG OPTIONAL_COMPONENTS Quick QuickControls2)
else()
    find_package(Qt6 CONFIG REQUIRED COMPONENTS Core Gui)
    find_package(Qt6 CONFIG OPTIONAL_COMPONENTS Quick QuickControls2)
endif()

# This module uses Qt private targets (e.g. Qt::CorePrivate) in multiple places.
# In an installed Qt build these targets live in separate *Private CMake packages,
# so we need to locate them explicitly (especially when building via add_subdirectory()).
find_package(Qt6CorePrivate CONFIG REQUIRED)
# Some platform backends reference DeviceDiscoverySupportPrivate (not built on Android,
# but harmless to locate if available).
find_package(Qt6DeviceDiscoverySupportPrivate CONFIG QUIET)

if(NOT TARGET Qt::Gui)
    message(NOTICE "Skipping the build as the condition \"TARGET Qt::Gui\" is not met.")
    return()
endif()

if(_qtgamepad_is_standalone)
    qt_build_repo()
else()
    # When vendored via add_subdirectory(), just include the source directories directly
    add_subdirectory(src)
endif()
