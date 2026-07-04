cmake_minimum_required(VERSION 3.20)
project(GetLight)

set(CMAKE_CXX_STANDARD 17)

add_executable(GetLight WIN32
        src/main.cpp
        src/App.cpp
        src/Config.cpp
)

target_include_directories(GetLight PRIVATE vendor)
