file(GLOB LOGIC_SOURCES CONFIGURE_DEPENDS  "src/*.cpp")

add_library(Logic_lib STATIC ${LOGIC_SOURCES})

target_include_directories(Logic_lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
