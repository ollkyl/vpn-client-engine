cmake_minimum_required(VERSION 3.25.0 FATAL_ERROR)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(NOT ENGINE_VERSION)
    set(ENGINE_VERSION 5.0.2.2 CACHE STRING "Engine version")
endif()

set(QT_CREATOR_SKIP_PACKAGE_MANAGER_SETUP ON CACHE BOOL "" FORCE)
set(CMAKE_PROJECT_TOP_LEVEL_INCLUDES
    ${CMAKE_SOURCE_DIR}/cmake/platform_settings.cmake
    ${CMAKE_SOURCE_DIR}/cmake/recipes_bootstrap.cmake
    ${CMAKE_SOURCE_DIR}/cmake/conan_provider.cmake
    CACHE STRING "" FORCE)

project(VpnEngine VERSION ${ENGINE_VERSION}
        DESCRIPTION "VPN client engine"
        LANGUAGES CXX
)

if(NOT WIN32)
    message(FATAL_ERROR "The engine supports Windows only")
endif()

# trigger conan to kick off `conan install` globally
find_package(OpenSSL REQUIRED)

set(APP_MAJOR_VERSION ${CMAKE_PROJECT_VERSION_MAJOR}.${CMAKE_PROJECT_VERSION_MINOR}.${CMAKE_PROJECT_VERSION_PATCH})
set(MZ_PLATFORM_NAME "windows")

include(${CMAKE_SOURCE_DIR}/client/cmake/branding.cmake)

add_subdirectory(client)
add_subdirectory(service)

# Scripts to register / remove the privileged service from the install folder
foreach(script install_service uninstall_service)
    configure_file(${CMAKE_SOURCE_DIR}/deploy/windows/${script}.cmd.in ${CMAKE_BINARY_DIR}/${script}.cmd @ONLY)
    install(FILES ${CMAKE_BINARY_DIR}/${script}.cmd DESTINATION ${CMAKE_INSTALL_BINDIR})
endforeach()
