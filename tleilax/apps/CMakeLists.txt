add_executable(tleilax-app tleilax-app.c)

set(SDL2_DIR "${Tleilax_SOURCE_DIR}/extern/SDL2-2.0.12")
find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIRS})

target_link_libraries(tleilax-app PRIVATE tleilax_library ${SDL2_LIBRARIES} lua_utils Lua raylib)

add_custom_command(
        TARGET tleilax-app POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
                ${CMAKE_SOURCE_DIR}/scripts
                ${CMAKE_CURRENT_BINARY_DIR}/scripts)
