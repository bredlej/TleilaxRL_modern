add_executable(test_tleilax test_tleilax.c "test_tleilax.c")
target_link_libraries(test_tleilax tleilax_library Unity)
add_executable(test_galaxy test_galaxy.c "test_galaxy.c")
target_link_libraries(test_galaxy galaxy Unity)
add_test(test_tleilax_all test_tleilax test_galaxy)

add_custom_command(
        TARGET test_galaxy POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/scripts
        ${CMAKE_CURRENT_BINARY_DIR}/scripts)