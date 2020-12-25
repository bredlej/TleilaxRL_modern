

#set(SDL2_DIR "${Tleilax_SOURCE_DIR}/extern/SDL2-2.0.12")
#find_package(SDL2 REQUIRED)
#include_directories(${SDL2_INCLUDE_DIRS})

if (${ENABLE_GRAPHICS})
    add_executable(tleilax-app tleilax-app.c)
    target_link_libraries(tleilax-app PRIVATE tleilax_library lua_utils Lua graphics)

    add_executable(orbit_test orbit_test.c)
    target_link_libraries(orbit_test PRIVATE raylib)

    add_executable(render_to_texture render_to_texture.c)
    target_link_libraries(render_to_texture PRIVATE raylib)

    add_executable(font_sizes font_sizes.c)
    target_link_libraries(font_sizes PRIVATE raylib)

    add_executable(star_shader star_shader.c)
    target_link_libraries(star_shader PRIVATE raylib)

    add_executable(instanced_rendering instanced_rendering.c)
    target_link_libraries(instanced_rendering PRIVATE raylib glfw)

    add_custom_command(
            TARGET tleilax-app POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_directory
            ${CMAKE_SOURCE_DIR}/scripts
            ${CMAKE_CURRENT_BINARY_DIR}/scripts)
    add_custom_command(
            TARGET tleilax-app POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_directory
            ${CMAKE_SOURCE_DIR}/assets
            ${CMAKE_CURRENT_BINARY_DIR}/assets)

endif ()

