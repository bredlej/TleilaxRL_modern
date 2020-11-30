set(HEADER_LIST "${Tleilax_SOURCE_DIR}/include/tleilax/tleilax.h")
set(GALAXY_HEADER "${Tleilax_SOURCE_DIR}/include/galaxy/galaxy.h")
set(STARSYSTEM_HEADER "${Tleilax_SOURCE_DIR}/include/galaxy/starsystem.h")
set(RANDOM_HEADER "${Tleilax_SOURCE_DIR}/include/random/random.h")
set(LUA_HEADERS "${Tleilax_SOURCE_DIR}/include/lua_utils/lua_utils.h")
set(ECS_HEADERS "${Tleilax_SOURCE_DIR}/include/ECS/ecs.h")

add_library (random random.c ${RANDOM_HEADER})
target_link_libraries(random PCG-random)
target_include_directories(random PUBLIC ../include)

add_library(lua_utils lua_utils.c ${LUA_HEADERS})
target_link_libraries(lua_utils Lua)
target_include_directories(lua_utils PUBLIC ../include)

add_library(ECS ecs.c ${ECS_HEADERS})
target_include_directories(ECS PUBLIC ../include)

add_library(galaxy galaxy.c ${GALAXY_HEADER})
target_include_directories(galaxy PUBLIC ../include)

add_library(galaxy_view state_views/galaxy_view.c "${Tleilax_SOURCE_DIR}/include/tleilax/views/galaxy_view.h")
target_link_libraries(galaxy_view raylib)
target_include_directories(galaxy_view PUBLIC ../include)

add_library(starsystem starsystem.c ${STARSYSTEM_HEADER})
target_link_libraries(starsystem ECS)
target_include_directories(starsystem PUBLIC ../include)

add_library(graphics graphics.c "${Tleilax_SOURCE_DIR}/include/tleilax/graphics.h")
target_link_libraries(graphics raylib galaxy_view)
target_include_directories(graphics PUBLIC ../include)


add_library(tleilax_library tleilax.c ${HEADER_LIST})
target_link_libraries(tleilax_library random galaxy starsystem C_StateMachine graphics)
target_include_directories(tleilax_library PRIVATE
        ${PROJECT_SOURCE_DIR}/include
        $<TARGET_PROPERTY:C_StateMachine,INCLUDE_DIRECTORIES>
        )

