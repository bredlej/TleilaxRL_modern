add_executable(test_tleilax test_tleilax.c "test_tleilax.c")
target_link_libraries(test_tleilax tleilax_library Unity C_StateMachine)

add_executable(test_galaxy test_galaxy.c "test_galaxy.c")
target_link_libraries(test_galaxy galaxy Unity)

add_executable(test_cstatemachine test_cstatemachine.c "test_cstatemachine.c")
target_link_libraries(test_cstatemachine C_StateMachine Unity)

add_executable(test_ecs test_ecs.c "test_ecs.c")
target_link_libraries(test_ecs ECS Unity)

add_executable(test_starsystem test_starsystem.c "test_starsystem.c")
target_link_libraries(test_starsystem ECS Unity galaxy starsystem)

add_test(test_tleilax_all test_tleilax test_galaxy test_cstatemachine)

add_custom_command(
        TARGET test_galaxy POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/scripts
        ${CMAKE_CURRENT_BINARY_DIR}/scripts)