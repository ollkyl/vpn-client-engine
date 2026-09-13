# Generated from sdl2.pro.

#####################################################################
## QSdl2GamepadBackendPlugin Plugin:
#####################################################################

qt_internal_add_plugin(QSdl2GamepadBackendPlugin
    OUTPUT_NAME sdl2gamepad
    PLUGIN_TYPE legacygamepads
    SOURCES
        main.cpp
        qsdlgamepadbackend.cpp qsdlgamepadbackend_p.h
        sdl2.json
    LIBRARIES
        Qt::Core
        Qt::GamepadLegacy
        Qt::GamepadLegacyPrivate
        Qt::Gui
        WrapSDL2::WrapSDL2
)

#### Keys ignored in scope 26:.:.:sdl2.pro:<TRUE>:
# OTHER_FILES = "sdl2.json"
