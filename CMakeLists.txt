project(seven-hearts)
cmake_minimum_required(VERSION 3.2)

if ("${CMAKE_BUILD_TYPE}" STREQUAL "")
	message(FATAL_ERROR "you need to specify a build type (-DCMAKE_BUILD_TYPE=<Debug|Release>)")
endif()

# include directories
include_directories(
	src
	ext/libuv)

# libuv
add_custom_command(
	OUTPUT ${CMAKE_SOURCE_DIR}/ext/libuv/build/gyp/gyp
	COMMAND git ARGS clone https://chromium.googlesource.com/external/gyp.git build/gyp
	WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/ext/libuv
	COMMENT "libuv: obtain gyp")

add_custom_command(
	OUTPUT ${CMAKE_SOURCE_DIR}/ext/libuv/out/${CMAKE_BUILD_TYPE}/build.ninja
	COMMAND python ARGS gyp_uv.py -f ninja
	DEPENDS ext/libuv/build/gyp/gyp
	WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/ext/libuv
	COMMENT "libuv: generate Ninja build configuration")

add_custom_target(
	OUTPUT ${CMAKE_SOURCE_DIR}/ext/libuv/out/${CMAKE_BUILD_TYPE}/libuv.a
	COMMAND ninja
	DEPENDS ext/libuv/out/${CMAKE_BUILD_TYPE}/build.ninja
	WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/ext/libuv/out/${CMAKE_BUILD_TYPE}
	COMMENT "libuv: build (${CMAKE_SOURCE_DIR}/ext/libuv/out/${CMAKE_BUILD_TYPE})")

add_custom_target(uv_lib ALL
	DEPENDS ${CMAKE_SOURCE_DIR}/ext/libuv/out/${CMAKE_BUILD_TYPE}/libuv.a)

add_library(uv STATIC IMPORTED)
set_property(TARGET uv APPEND PROPERTY IMPORTED_CONFIGURATIONS NOCONFIG)
set_target_properties(uv PROPERTIES
	IMPORTED_LOCATION_NOCONFIG "${CMAKE_SOURCE_DIR}/ext/libuv/out/${CMAKE_BUILD_TYPE}/libuv.a")

# common sources
set(COMMON_SRC
	src/common/log.cxx)

# login server
set(LOGIN_SRC
	${COMMON_SRC}
	src/login/main.cxx)

add_executable(svh-login ${LOGIN_SRC})

# character server
set(CHAR_SRC
	${COMMON_SRC}
	src/character/main.cxx)

add_executable(svh-char ${CHAR_SRC})

# world server
set(WORLD_SRC
	${COMMON_SRC}
	src/world/main.cxx)

add_executable(svh-world ${WORLD_SRC})

# linkage
target_link_libraries(svh-login uv)
target_link_libraries(svh-char uv)
target_link_libraries(svh-world uv)
