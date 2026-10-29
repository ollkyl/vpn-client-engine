include_directories(${CMAKE_CURRENT_LIST_DIR})

set(HEADERS ${HEADERS}
    ${CMAKE_CURRENT_LIST_DIR}/qtservice.h
    ${CMAKE_CURRENT_LIST_DIR}/qtservice_p.h
)

set(SOURCES ${SOURCES}
    ${CMAKE_CURRENT_LIST_DIR}/qtservice.cpp
    ${CMAKE_CURRENT_LIST_DIR}/qtservice_win.cpp
)
