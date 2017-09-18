cmake_minimum_required(VERSION 2.8)

project(pastec)

set(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -std=c++1z -Wall")
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


link_directories(/usr/lib/gcc/x86_64-linux-gnu/7)

#target_compile_options(${PROJECT_NAME} PUBLIC -std=c++1z -Wall -Wfloat-conversion)

add_executable(${PROJECT_NAME} ${SOURCES} )

target_link_libraries(${PROJECT_NAME} pthread fastcgipp stdc++)

