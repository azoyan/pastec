cmake_minimum_required(VERSION 2.8)

project(pastec)

set(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS}")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wall -Wpedantic -Wextra")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O0 -ggdb")


set(SOURCES
    main.cpp	
    Storage.h
    Storage.cpp
    Request.h
    Request.cpp
    Session.hpp
    Dictionary.hpp
)


add_executable(${PROJECT_NAME} ${SOURCES} )

target_link_libraries(${PROJECT_NAME} pthread fastcgipp)
set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 14)

