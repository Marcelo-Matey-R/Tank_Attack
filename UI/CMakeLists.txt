file(GLOB UI_SOURCES CONFIGURE_DEPENDS  "src/*.cpp")

add_library(UI_lib STATIC ${UI_SOURCES})

target_include_directories(UI_lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
