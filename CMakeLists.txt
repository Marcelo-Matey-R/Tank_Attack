cmake_minimum_required(VERSION 3.25)
project(Tank_Attack LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(SFML 2.6 COMPONENTS graphics window system REQUIRED)

#add_subdirectory(Logic)
add_subdirectory(UI)

add_executable(TankAttack main.cpp Game.cpp)

target_include_directories(TankAttack PRIVATE include)

target_link_libraries(TankAttack
    PRIVATE
    UI_lib
    #Logic_lib
    sfml-graphics
    sfml-window
    sfml-system
)

file(COPY assets/ DESTINATION ${CMAKE_BINARY_DIR}/assets)