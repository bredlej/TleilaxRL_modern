add_library(Unity STATIC Unity/src/unity.c)
target_include_directories(Unity PUBLIC Unity/src)

add_library(PCG-random STATIC pcg-c-basic-0.9/pcg_basic.c)
target_include_directories(PCG-random PUBLIC pcg-c-basic-0.9)

SET(LUA_SRC_LIB
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

add_library(FaultAllocator STATIC C_StateMachine/Fault.c)
target_include_directories(FaultAllocator PUBLIC C_StateMachine)

add_library(FBAllocator STATIC C_StateMachine/fb_allocator.c)
target_include_directories(FBAllocator PUBLIC C_StateMachine)
target_link_libraries(FBAllocator)

SET(C_STATEMACHINE_SRC_LIB
        C_StateMachine/StateMachine.c
        #C_StateMachine/Fault.cpp
        #C_StateMachine/fb_allocator.c
        #C_StateMachine/LockGuard.c
        C_StateMachine/sm_allocator.c
        C_StateMachine/x_allocator.c
        )

add_library(C_StateMachine STATIC ${C_STATEMACHINE_SRC_LIB})
target_include_directories(C_StateMachine PUBLIC C_StateMachine)
target_link_libraries(C_StateMachine FaultAllocator FBAllocator)

if (${ENABLE_GRAPHICS})
    # Set this to the minimal version you want to support
    find_package(raylib 3.0 QUIET) # Let CMake search for a raylib-config.cmake

    # You could change the QUIET above to REQUIRED and remove this if() clause
    # This part downloads raylib and builds it if it's not installed on your system
    if (NOT raylib_FOUND) # If there's none, fetch and build raylib
        include(FetchContent)

        FetchContent_Declare(
                raylib
                GIT_REPOSITORY https://github.com/raysan5/raylib.git
                GIT_TAG 3.0.0
        )

        FetchContent_GetProperties(raylib)
        if (NOT raylib_POPULATED) # Have we downloaded raylib yet?
            set(FETCHCONTENT_QUIET NO)
            FetchContent_Populate(raylib)

            set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE) # don't build the supplied examples

            # build raylib
            add_subdirectory(${raylib_SOURCE_DIR} ${raylib_BINARY_DIR})

        endif ()

    endif ()


endif ()