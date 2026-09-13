# Generated from darwin.pro.

#####################################################################
## QDarwinGamepadBackendPlugin Plugin:
#####################################################################

qt_internal_add_plugin(QDarwinGamepadBackendPlugin
    OUTPUT_NAME darwingamepad
    PLUGIN_TYPE legacygamepads
    SOURCES
        main.cpp
        qdarwingamepadbackend.mm qdarwingamepadbackend_p.h
        darwin.json
    LIBRARIES
        ${FWFoundation}
        ${FWGameController}
        Qt::Core
        Qt::GamepadLegacy
        Qt::GamepadLegacyPrivate
        Qt::Gui
)

#### Keys ignored in scope 18:.:.:darwin.pro:<TRUE>:
# OTHER_FILES = "darwin.json"
# PLUGIN_EXTENDS = "gamepad"
