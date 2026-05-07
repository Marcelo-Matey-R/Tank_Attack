cmake_minimum_required(VERSION 3.25)
project(Tank_Attack LANGUAGES CXX )

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_subdirectory(Logic)
add_subdirectory(UI)

add_executable(TankAttack src/main.cpp src/Game.cpp)

target_include_directories(TankAttack PRIVATE include)
