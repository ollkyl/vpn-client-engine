# Generated from jar.pro.

#
# Qt <= 6.9 provided `qt_get_android_sdk_jar_for_api()`. In Qt 6.10 this helper
# is no longer available in many setups, but we still need an `android.jar` to
# compile the Java sources for the jar.
#
# Keep backward compatibility when the Qt helper exists; otherwise resolve
# android.jar from the local Android SDK installation.
#
if(COMMAND qt_get_android_sdk_jar_for_api)
qt_get_android_sdk_jar_for_api("android-16" android_sdk)
else()
    # Try to locate the Android SDK root.
    set(_qtgamepad_android_sdk_root "")
    foreach(_v IN ITEMS ANDROID_SDK_ROOT ANDROID_HOME QT_ANDROID_SDK_ROOT)
        if(DEFINED ${_v} AND NOT "${${_v}}" STREQUAL "")
            set(_qtgamepad_android_sdk_root "${${_v}}")
            break()
        endif()
        if(DEFINED ENV{${_v}} AND NOT "$ENV{${_v}}" STREQUAL "")
            set(_qtgamepad_android_sdk_root "$ENV{${_v}}")
            break()
        endif()
    endforeach()

    if(_qtgamepad_android_sdk_root STREQUAL "")
        message(FATAL_ERROR
            "Unable to locate Android SDK root (tried ANDROID_SDK_ROOT / ANDROID_HOME / QT_ANDROID_SDK_ROOT). "
            "Set one of these variables so we can find android.jar.")
    endif()

    # Prefer the historical platform if installed; otherwise fall back to the
    # newest installed android-XX platform (more robust on modern SDK installs).
    set(_qtgamepad_preferred_platform "android-16")
    set(_qtgamepad_candidate_jar
        "${_qtgamepad_android_sdk_root}/platforms/${_qtgamepad_preferred_platform}/android.jar")

    if(EXISTS "${_qtgamepad_candidate_jar}")
        set(android_sdk "${_qtgamepad_candidate_jar}")
    else()
        file(GLOB _qtgamepad_android_jars
            LIST_DIRECTORIES false
            "${_qtgamepad_android_sdk_root}/platforms/android-*/android.jar")

        if(_qtgamepad_android_jars STREQUAL "")
            message(FATAL_ERROR
                "Could not find any android.jar under '${_qtgamepad_android_sdk_root}/platforms'. "
                "Install at least one Android platform (SDK Platforms) in the Android SDK manager.")
        endif()

        # Pick the highest API level available.
        set(_qtgamepad_best_api -1)
        set(_qtgamepad_best_jar "")
        foreach(_jar IN LISTS _qtgamepad_android_jars)
            # .../platforms/android-34/android.jar
            get_filename_component(_platform_dir "${_jar}" DIRECTORY)
            get_filename_component(_platform_name "${_platform_dir}" NAME)
            string(REGEX REPLACE "^android-" "" _api_str "${_platform_name}")
            if(_api_str MATCHES "^[0-9]+$")
                if(_api_str GREATER _qtgamepad_best_api)
                    set(_qtgamepad_best_api "${_api_str}")
                    set(_qtgamepad_best_jar "${_jar}")
                endif()
            endif()
        endforeach()

        if(_qtgamepad_best_jar STREQUAL "")
            message(FATAL_ERROR
                "Found android.jar candidates but could not parse API levels under '${_qtgamepad_android_sdk_root}/platforms'.")
        endif()

        set(android_sdk "${_qtgamepad_best_jar}")
    endif()
endif()

set(java_sources
    src/org/qtproject/qt/android/gamepad/QtGamepad.java
)

qt_internal_add_jar(QtAndroidGamepad
    INCLUDE_JARS ${android_sdk}
    SOURCES ${java_sources}
    OUTPUT_DIR "${QT_BUILD_DIR}/jar"
)

install_jar(QtAndroidGamepad
    DESTINATION jar
    COMPONENT Devel
)

