set(HEADER_LIST "${Tleilax_SOURCE_DIR}/include/tleilax/tleilax.h")
set(LUA_HEADERS "${Tleilax_SOURCE_DIR}/include/lua_utils/lua_utils.h")

add_library(lua_utils lua_utils.c ${LUA_HEADERS})
target_link_libraries(lua_utils Lua)
target_include_directories(lua_utils PUBLIC ../include)

add_library(tleilax_library tleilax.c ${HEADER_LIST})
target_link_libraries(tleilax_library PCG-random)
target_include_directories(tleilax_library PUBLIC ../include)