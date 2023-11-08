cmake_minimum_required(VERSION 3.0)
project(mapping)

set(CMAKE_PREFIX_PATH ../../sdk)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)
set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_CXX_COMPILER "clang++")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -stdlib=libc++ -femit-all-decls -fstandalone-debug")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -stdlib=libc++")

include_directories(include minecraft-headers)

find_package(ModLoader REQUIRED)

add_library(mapping SHARED src/main.cpp)
target_link_libraries(mapping PUBLIC ModLoader)
