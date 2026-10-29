cmake_minimum_required(VERSION 3.25.0 FATAL_ERROR)

project(vpn_engine LANGUAGES CXX)

include(${CMAKE_CURRENT_LIST_DIR}/cmake/branding.cmake)

find_package(Qt6 REQUIRED COMPONENTS Core Network RemoteObjects Core5Compat Concurrent)
qt_standard_project_setup()

include(${CMAKE_CURRENT_LIST_DIR}/cmake/3rdparty.cmake)
include(${CMAKE_CURRENT_LIST_DIR}/cmake/sources.cmake)

configure_file(${CMAKE_CURRENT_LIST_DIR}/../version.h.in ${CMAKE_CURRENT_BINARY_DIR}/version.h)

qt_add_library(vpn_engine STATIC ${ENGINE_SOURCES})

qt_add_repc_replicas(vpn_engine
    ${CMAKE_CURRENT_LIST_DIR}/../ipc/ipc_interface.rep
    ${CMAKE_CURRENT_LIST_DIR}/../ipc/ipc_process_interface.rep
)

target_include_directories(vpn_engine PUBLIC
    ${ENGINE_INCLUDE_DIRS}
    ${CMAKE_CURRENT_BINARY_DIR}
)

target_compile_definitions(vpn_engine
    PUBLIC
        AMNEZIA_DESKTOP
        "MZ_$<UPPER_CASE:${MZ_PLATFORM_NAME}>"
        $<$<CONFIG:Debug>:MZ_DEBUG>
)

target_link_libraries(vpn_engine PUBLIC
    Qt6::Core
    Qt6::Network
    Qt6::RemoteObjects
    Qt6::Core5Compat
    Qt6::Concurrent
    qt6keychain
    OpenSSL::SSL
    OpenSSL::Crypto
)

if(WIN32)
    # See the note in service/server/CMakeLists.txt: keeps <windows.h> min/max macros from breaking Qt headers
    target_compile_definitions(vpn_engine PUBLIC NOMINMAX _WINSOCKAPI_)
    target_link_libraries(vpn_engine PUBLIC user32 rasapi32 shlwapi iphlpapi ws2_32)
endif()

add_subdirectory(cli)
