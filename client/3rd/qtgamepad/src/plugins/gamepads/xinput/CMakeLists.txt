# Generated from xinput.pro.

#####################################################################
## QXInputGamepadBackendPlugin Plugin:
#####################################################################

qt_internal_add_plugin(QXInputGamepadBackendPlugin
    OUTPUT_NAME xinputgamepad
    PLUGIN_TYPE legacygamepads
    SOURCES
        main.cpp
        qxinputgamepadbackend.cpp qxinputgamepadbackend_p.h
        xinput.json
    LIBRARIES
        Qt::Core
        Qt::GamepadLegacy
        Qt::GamepadLegacyPrivate
        Qt::Gui
)

#### Keys ignored in scope 27:.:.:xinput.pro:<TRUE>:
# OTHER_FILES = "xinput.json"
