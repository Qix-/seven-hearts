project(seven-hearts)
cmake_minimum_required(VERSION 3.2)

if ("${CMAKE_BUILD_TYPE}" STREQUAL "")
	message(FATAL_ERROR "you need to specify a build type (-DCMAKE_BUILD_TYPE=<Debug|Release>)")
endif()

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

add_custom_target(uv ALL
	COMMAND ninja
	DEPENDS ext/libuv/out/${CMAKE_BUILD_TYPE}/build.ninja
	WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/ext/libuv/out/${CMAKE_BUILD_TYPE}
	COMMENT "libuv: build (${CMAKE_SOURCE_DIR}/ext/libuv/out/${CMAKE_BUILD_TYPE})")
