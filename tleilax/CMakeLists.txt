cmake_minimum_required(VERSION 3.11)

project(
	Tleilax
	VERSION 0.1
	DESCRIPTION "Empty project template"
	LANGUAGES C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_FLAGS "-Wall -Wno-error")
set(ENABLE_GRAPHICS 1)

enable_testing()

add_subdirectory(extern)
add_subdirectory(src)
add_subdirectory(apps)
add_subdirectory(tests)

#include_directories(${SDL2_INCLUDE_DIRS})
