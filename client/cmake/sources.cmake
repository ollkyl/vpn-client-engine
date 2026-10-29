# Sources of the vpn_engine library.
set(ENGINE_ROOT_DIR ${CMAKE_CURRENT_LIST_DIR}/..)

file(GLOB_RECURSE ENGINE_CORE_SOURCES CONFIGURE_DEPENDS
    ${ENGINE_ROOT_DIR}/core/*.cpp
    ${ENGINE_ROOT_DIR}/core/*.h
)
# Windows-only protocol, added below
list(FILTER ENGINE_CORE_SOURCES EXCLUDE REGEX ".*/ikev2VpnProtocolWindows\\.(h|cpp)$")

set(ENGINE_SOURCES
    ${ENGINE_CORE_SOURCES}

    ${ENGINE_ROOT_DIR}/vpnEngine.h
    ${ENGINE_ROOT_DIR}/vpnEngine.cpp
    ${ENGINE_ROOT_DIR}/vpnConnection.h
    ${ENGINE_ROOT_DIR}/vpnConnection.cpp
    ${ENGINE_ROOT_DIR}/secureQSettings.h
    ${ENGINE_ROOT_DIR}/secureQSettings.cpp

    ${ENGINE_ROOT_DIR}/../common/logger/logger.h
    ${ENGINE_ROOT_DIR}/../common/logger/logger.cpp
    ${ENGINE_ROOT_DIR}/../common/crypto/cryptoUtils.h
    ${ENGINE_ROOT_DIR}/../common/crypto/cryptoUtils.cpp
    ${ENGINE_ROOT_DIR}/../ipc/ipc.h

    ${ENGINE_ROOT_DIR}/mozilla/controllerimpl.h
    ${ENGINE_ROOT_DIR}/mozilla/localsocketcontroller.h
    ${ENGINE_ROOT_DIR}/mozilla/localsocketcontroller.cpp
    ${ENGINE_ROOT_DIR}/mozilla/models/server.h
    ${ENGINE_ROOT_DIR}/mozilla/models/server.cpp
    ${ENGINE_ROOT_DIR}/mozilla/shared/ipaddress.h
    ${ENGINE_ROOT_DIR}/mozilla/shared/ipaddress.cpp
    ${ENGINE_ROOT_DIR}/mozilla/shared/leakdetector.h
    ${ENGINE_ROOT_DIR}/mozilla/shared/leakdetector.cpp
)

if(WIN32)
    list(APPEND ENGINE_SOURCES
        ${ENGINE_ROOT_DIR}/core/protocols/ikev2VpnProtocolWindows.h
        ${ENGINE_ROOT_DIR}/core/protocols/ikev2VpnProtocolWindows.cpp
    )
endif()

set(ENGINE_INCLUDE_DIRS
    ${ENGINE_ROOT_DIR}
    ${ENGINE_ROOT_DIR}/mozilla
    ${ENGINE_ROOT_DIR}/mozilla/shared
    ${ENGINE_ROOT_DIR}/mozilla/models
    ${ENGINE_ROOT_DIR}/../ipc
    ${ENGINE_ROOT_DIR}/../common/logger
    ${ENGINE_ROOT_DIR}/../common/crypto
)
