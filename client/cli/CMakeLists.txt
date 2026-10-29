# Minimal console client: imports a config and connects, without any UI.
# Use it to check that the engine works, and as an example of the VpnEngine API.
qt_add_executable(vpn-engine-cli main.cpp)

target_link_libraries(vpn-engine-cli PRIVATE vpn_engine)

install(TARGETS vpn-engine-cli
    DESTINATION ${CMAKE_INSTALL_BINDIR}
    RUNTIME_DEPENDENCY_SET cli_deps
)

install(RUNTIME_DEPENDENCY_SET cli_deps
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
    TARGET vpn-engine-cli
    OUTPUT_SCRIPT CLI_DEPLOY_SCRIPT
    NO_UNSUPPORTED_PLATFORM_ERROR
)
install(SCRIPT ${CLI_DEPLOY_SCRIPT})
