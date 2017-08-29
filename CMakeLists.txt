cmake_minimum_required(VERSION 2.8)

project(pastec)

set(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -std=c++11 -Wall")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wall -Wpedantic -Wextra")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O0 -ggdb")


set(SOURCES
   src/main.cpp
    src/Storage.h
    src/Storage.cpp
    src/Request.h
    src/Request.cpp
    src/Session.hpp
    src/Dictionary.hpp
)

#target_compile_options(${PROJECT_NAME} PUBLIC -std=c++1y -Wall -Wfloat-conversion)

add_executable(${PROJECT_NAME} ${SOURCES} )

target_link_libraries(${PROJECT_NAME} pthread fastcgipp)
set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 14)

