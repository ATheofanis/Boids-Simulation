cmake_minimum_required(VERSION 3.20)
project(Boids CXX)

set(CMAKE_C_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


list(APPEND CMAKE_PREFIX_PATH "C:/msys64/mingw64")

find_package(raylib REQUIRED)


add_executable(${PROJECT_NAME} src/main.cpp
        src/Parameters.h
)

target_link_libraries(${PROJECT_NAME} raylib)