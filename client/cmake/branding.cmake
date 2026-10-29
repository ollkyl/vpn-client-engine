# Names used for the client executable, the privileged service, the settings storage and the keychain.
# Override them from the command line (-DCLIENT_APPLICATION_NAME=MyVPN ...) or before add_subdirectory().
# Changing them gives your product its own service and settings, independent from other installations.

if(NOT CLIENT_APPLICATION_NAME)
    set(CLIENT_APPLICATION_NAME "VpnEngine" CACHE STRING "Application name: settings, log and config file names")
endif()
if(NOT CLIENT_SERVICE_NAME)
    set(CLIENT_SERVICE_NAME "VpnEngine-service" CACHE STRING "Privileged service executable name")
endif()
if(NOT CLIENT_ORGANIZATION_NAME)
    set(CLIENT_ORGANIZATION_NAME "VpnEngine" CACHE STRING "QSettings organization name")
endif()
if(NOT CLIENT_KEYCHAIN_NAME)
    set(CLIENT_KEYCHAIN_NAME "${CLIENT_APPLICATION_NAME}-Keychain" CACHE STRING "Keychain service name for the settings encryption key")
endif()
