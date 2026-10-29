cmake_minimum_required(VERSION 3.25.0 FATAL_ERROR)

set(PROJECT ${CLIENT_SERVICE_NAME})
project(${PROJECT} VERSION ${ENGINE_VERSION})

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Core Network Widgets RemoteObjects Core5Compat Concurrent)
qt_standard_project_setup()

configure_file(${CMAKE_SOURCE_DIR}/version.h.in ${CMAKE_CURRENT_BINARY_DIR}/version.h)

set(CLIENT_DIR ${CMAKE_CURRENT_LIST_DIR}/../../client)

set(HEADERS
    ${CLIENT_DIR}/core/utils/utilities.h
    ${CLIENT_DIR}/core/utils/networkUtilities.h
    ${CMAKE_CURRENT_LIST_DIR}/../../ipc/ipc.h
    ${CMAKE_CURRENT_LIST_DIR}/../../ipc/ipcserver.h
    ${CMAKE_CURRENT_LIST_DIR}/../../ipc/ipcserverprocess.h
    ${CMAKE_CURRENT_LIST_DIR}/localserver.h
    ${CMAKE_CURRENT_LIST_DIR}/../../common/logger/logger.h
    ${CMAKE_CURRENT_LIST_DIR}/router.h
    ${CMAKE_CURRENT_LIST_DIR}/killswitch.h
    ${CMAKE_CURRENT_LIST_DIR}/systemservice.h
    ${CMAKE_CURRENT_LIST_DIR}/xray.h
    ${CMAKE_CURRENT_LIST_DIR}/tapcontroller_win.h
    ${CMAKE_CURRENT_LIST_DIR}/router_win.h
    ${CMAKE_CURRENT_BINARY_DIR}/version.h

    ${CLIENT_DIR}/daemon/daemon.h
    ${CLIENT_DIR}/daemon/daemonerrors.h
    ${CLIENT_DIR}/daemon/daemonlocalserver.h
    ${CLIENT_DIR}/daemon/daemonlocalserverconnection.h
    ${CLIENT_DIR}/daemon/dnsutils.h
    ${CLIENT_DIR}/daemon/iputils.h
    ${CLIENT_DIR}/daemon/interfaceconfig.h
    ${CLIENT_DIR}/daemon/wireguardutils.h

    ${CLIENT_DIR}/platforms/dummy/dummynetworkwatcher.h

    ${CLIENT_DIR}/mozilla/shared/ipaddress.h
    ${CLIENT_DIR}/mozilla/shared/loglevel.h
    ${CLIENT_DIR}/mozilla/shared/leakdetector.h
    ${CLIENT_DIR}/mozilla/models/server.h
    ${CLIENT_DIR}/mozilla/controllerimpl.h
    ${CLIENT_DIR}/mozilla/dnspingsender.h
    ${CLIENT_DIR}/mozilla/networkwatcher.h
    ${CLIENT_DIR}/mozilla/networkwatcherimpl.h
    ${CLIENT_DIR}/mozilla/pinghelper.h
    ${CLIENT_DIR}/mozilla/pingsender.h
    ${CLIENT_DIR}/mozilla/pingsenderfactory.h

    ${CLIENT_DIR}/platforms/windows/daemon/windowsdaemon.h
    ${CLIENT_DIR}/platforms/windows/daemon/windowsdaemontunnel.h
    ${CLIENT_DIR}/platforms/windows/daemon/windowsfirewall.h
    ${CLIENT_DIR}/platforms/windows/daemon/dnsutilswindows.h
    ${CLIENT_DIR}/platforms/windows/daemon/windowssplittunnel.h
    ${CLIENT_DIR}/platforms/windows/daemon/windowstunnelservice.h
    ${CLIENT_DIR}/platforms/windows/daemon/wireguardutilswindows.h
    ${CLIENT_DIR}/platforms/windows/daemon/windowstunnellogger.h
    ${CLIENT_DIR}/platforms/windows/daemon/windowsroutemonitor.h
    ${CLIENT_DIR}/platforms/windows/windowscommons.h
    ${CLIENT_DIR}/platforms/windows/windowsservicemanager.h
    ${CLIENT_DIR}/platforms/windows/windowsutils.h
    ${CLIENT_DIR}/platforms/windows/windowspingsender.h
    ${CLIENT_DIR}/platforms/windows/windowsnetworkwatcher.h
)

set(SOURCES
    ${CLIENT_DIR}/core/utils/utilities.cpp
    ${CLIENT_DIR}/core/utils/networkUtilities.cpp
    ${CMAKE_CURRENT_LIST_DIR}/../../ipc/ipcserver.cpp
    ${CMAKE_CURRENT_LIST_DIR}/../../ipc/ipcserverprocess.cpp
    ${CMAKE_CURRENT_LIST_DIR}/localserver.cpp
    ${CMAKE_CURRENT_LIST_DIR}/../../common/logger/logger.cpp
    ${CMAKE_CURRENT_LIST_DIR}/main.cpp
    ${CMAKE_CURRENT_LIST_DIR}/router.cpp
    ${CMAKE_CURRENT_LIST_DIR}/killswitch.cpp
    ${CMAKE_CURRENT_LIST_DIR}/systemservice.cpp
    ${CMAKE_CURRENT_LIST_DIR}/xray.cpp
    ${CMAKE_CURRENT_LIST_DIR}/tapcontroller_win.cpp
    ${CMAKE_CURRENT_LIST_DIR}/router_win.cpp

    ${CLIENT_DIR}/daemon/daemon.cpp
    ${CLIENT_DIR}/daemon/daemonlocalserver.cpp
    ${CLIENT_DIR}/daemon/daemonlocalserverconnection.cpp
    ${CLIENT_DIR}/daemon/interfaceconfig.cpp

    ${CLIENT_DIR}/platforms/dummy/dummynetworkwatcher.cpp

    ${CLIENT_DIR}/mozilla/models/server.cpp
    ${CLIENT_DIR}/mozilla/shared/ipaddress.cpp
    ${CLIENT_DIR}/mozilla/shared/leakdetector.cpp
    ${CLIENT_DIR}/mozilla/dnspingsender.cpp
    ${CLIENT_DIR}/mozilla/networkwatcher.cpp
    ${CLIENT_DIR}/mozilla/pinghelper.cpp
    ${CLIENT_DIR}/mozilla/pingsender.cpp
    ${CLIENT_DIR}/mozilla/pingsenderfactory.cpp

    ${CLIENT_DIR}/platforms/windows/daemon/windowsdaemon.cpp
    ${CLIENT_DIR}/platforms/windows/daemon/windowsdaemontunnel.cpp
    ${CLIENT_DIR}/platforms/windows/daemon/windowsfirewall.cpp
    ${CLIENT_DIR}/platforms/windows/daemon/dnsutilswindows.cpp
    ${CLIENT_DIR}/platforms/windows/daemon/windowssplittunnel.cpp
    ${CLIENT_DIR}/platforms/windows/daemon/windowstunnelservice.cpp
    ${CLIENT_DIR}/platforms/windows/daemon/wireguardutilswindows.cpp
    ${CLIENT_DIR}/platforms/windows/daemon/windowstunnellogger.cpp
    ${CLIENT_DIR}/platforms/windows/daemon/windowsroutemonitor.cpp
    ${CLIENT_DIR}/platforms/windows/windowscommons.cpp
    ${CLIENT_DIR}/platforms/windows/windowsservicemanager.cpp
    ${CLIENT_DIR}/platforms/windows/windowspingsender.cpp
    ${CLIENT_DIR}/platforms/windows/windowsnetworkwatcher.cpp
    ${CLIENT_DIR}/platforms/windows/windowsutils.cpp
)

configure_file(
    ${CMAKE_CURRENT_LIST_DIR}/service.rc.in
    ${CMAKE_CURRENT_BINARY_DIR}/service.rc
)
set(RESOURCES ${CMAKE_CURRENT_BINARY_DIR}/service.rc)

include(${CMAKE_CURRENT_LIST_DIR}/../src/qtservice.cmake)

add_executable(${PROJECT} ${SOURCES} ${HEADERS} ${RESOURCES})

target_include_directories(${PROJECT} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}
    ${CLIENT_DIR}
    ${CLIENT_DIR}/mozilla
    ${CLIENT_DIR}/mozilla/shared
    ${CLIENT_DIR}/mozilla/models
    ${CLIENT_DIR}/platforms
    ${CMAKE_CURRENT_LIST_DIR}/../../ipc
    ${CMAKE_CURRENT_LIST_DIR}/../../common/logger
    ${CMAKE_CURRENT_LIST_DIR}/../src
    ${CMAKE_CURRENT_BINARY_DIR}
)

# Without NOMINMAX the first header that drags in <windows.h> decides whether its min/max macros
# break Qt template headers later in the same translation unit, so define it project-wide.
target_compile_definitions(${PROJECT} PRIVATE
    NOMINMAX
    _WINSOCKAPI_
    "MZ_$<UPPER_CASE:${MZ_PLATFORM_NAME}>"
    $<$<CONFIG:Debug>:MZ_DEBUG>
)

target_link_libraries(${PROJECT} PRIVATE
    Qt6::Core Qt6::Widgets Qt6::Network Qt6::RemoteObjects Qt6::Core5Compat Qt6::Concurrent
    user32 rasapi32 shlwapi iphlpapi ws2_32 gdi32 Advapi32 Kernel32
)

find_package(amnezia-xray-bindings REQUIRED)
target_link_libraries(${PROJECT} PRIVATE amnezia::xray-bindings)

find_package(OpenSSL REQUIRED)
target_link_libraries(${PROJECT} PRIVATE OpenSSL::SSL OpenSSL::Crypto)

qt_add_repc_sources(${PROJECT} ${CMAKE_CURRENT_LIST_DIR}/../../ipc/ipc_interface.rep)
qt_add_repc_sources(${PROJECT} ${CMAKE_CURRENT_LIST_DIR}/../../ipc/ipc_process_interface.rep)

# VPN backends started by the service: they are copied next to the service executable
find_package(awg-windows REQUIRED)
find_package(wintun REQUIRED)
find_package(openvpn REQUIRED)
find_package(tun2socks REQUIRED)
find_package(v2ray-rules-dat REQUIRED)

set(BACKEND_EXECS
    $<TARGET_FILE:amnezia::awg-windows>
    $<TARGET_FILE:openvpn::openvpn>
    $<TARGET_FILE:xjasonlyu::tun2socks>
)
set(BACKEND_FILES
    $<TARGET_FILE:zx2c4::wintun>
    $<TARGET_FILE:OpenSSL::SSL>
    $<TARGET_FILE:OpenSSL::Crypto>
    ${GEOSITE_DAT_PATH}
    ${GEOIP_DAT_PATH}
)

add_custom_command(TARGET ${PROJECT} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        ${BACKEND_EXECS} ${BACKEND_FILES}
        "$<TARGET_FILE_DIR:${PROJECT}>"
)
# PERMISSIONS is not used here: on Windows it marks the files read-only
install(FILES ${BACKEND_EXECS} ${BACKEND_FILES}
    DESTINATION ${CMAKE_INSTALL_BINDIR}
)

# Drivers
find_package(tap-windows6 REQUIRED)
add_custom_command(TARGET ${PROJECT} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${TAP_WINDOWS6_BIN}"
        "$<TARGET_FILE_DIR:${PROJECT}>/tap"
)
install(DIRECTORY "${TAP_WINDOWS6_BIN}/"
    DESTINATION "${CMAKE_INSTALL_BINDIR}/tap"
)

find_package(win-split-tunnel REQUIRED)
add_custom_command(TARGET ${PROJECT} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${WIN_SPLIT_TUNNEL_BIN}"
        "$<TARGET_FILE_DIR:${PROJECT}>"
)
install(DIRECTORY "${WIN_SPLIT_TUNNEL_BIN}/"
    DESTINATION ${CMAKE_INSTALL_BINDIR}
)

install(TARGETS ${PROJECT}
    DESTINATION ${CMAKE_INSTALL_BINDIR}
    RUNTIME_DEPENDENCY_SET service_deps
)

install(RUNTIME_DEPENDENCY_SET service_deps
    PRE_EXCLUDE_REGEXES
        [[api-ms-win-.*]]
        [[ext-ms-.*]]
        [[kernel32\.dll]]
        [[hvsifiletrust\.dll]]
        [[^(lib)?[Qq]t.*]]
    POST_EXCLUDE_REGEXES
        [[^.*[\\/]system32[\\/].*\.dll$]]
    DIRECTORIES ${CONAN_RUNTIME_LIB_DIRS}
    DESTINATION ${CMAKE_INSTALL_BINDIR}
)

qt_generate_deploy_app_script(
    TARGET ${PROJECT}
    OUTPUT_SCRIPT QT_DEPLOY_SCRIPT
    NO_UNSUPPORTED_PLATFORM_ERROR
)
install(SCRIPT ${QT_DEPLOY_SCRIPT})
