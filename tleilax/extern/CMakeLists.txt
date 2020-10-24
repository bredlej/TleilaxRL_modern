add_library(Unity STATIC Unity/src/unity.c)
target_include_directories(Unity PUBLIC Unity/src)

add_library(PCG-random STATIC pcg-c-basic-0.9/pcg_basic.c)
target_include_directories(PCG-random PUBLIC pcg-c-basic-0.9)

SET (   LUA_SRC_LIB 
		lua-5.4.1/src/lapi.c
		lua-5.4.1/src/lauxlib.c
		lua-5.4.1/src/lbaselib.c
		lua-5.4.1/src/lcode.c
		lua-5.4.1/src/lcorolib.c
		lua-5.4.1/src/lctype.c
		lua-5.4.1/src/ldblib.c
		lua-5.4.1/src/ldebug.c
		lua-5.4.1/src/ldo.c
		lua-5.4.1/src/ldump.c
		lua-5.4.1/src/lfunc.c
		lua-5.4.1/src/lgc.c
		lua-5.4.1/src/linit.c
		lua-5.4.1/src/liolib.c
		lua-5.4.1/src/llex.c
		lua-5.4.1/src/lmathlib.c
		lua-5.4.1/src/lmem.c
		lua-5.4.1/src/loadlib.c
		lua-5.4.1/src/lobject.c
		lua-5.4.1/src/lopcodes.c
		lua-5.4.1/src/loslib.c
		lua-5.4.1/src/lparser.c
		lua-5.4.1/src/lstate.c
		lua-5.4.1/src/lstring.c
		lua-5.4.1/src/lstrlib.c
		lua-5.4.1/src/ltable.c
		lua-5.4.1/src/ltablib.c
		lua-5.4.1/src/ltm.c
		lua-5.4.1/src/lundump.c
		lua-5.4.1/src/lutf8lib.c
		lua-5.4.1/src/lvm.c
		lua-5.4.1/src/lzio.c
		)
add_library(Lua STATIC ${LUA_SRC_LIB})
target_include_directories(Lua PUBLIC lua-5.4.1/src)