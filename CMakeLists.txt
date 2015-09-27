project(seven-hearts)
cmake_minimum_required(VERSION 3.2)

if ("${CMAKE_BUILD_TYPE}" STREQUAL "")
	message(FATAL_ERROR "you need to specify a build type (-DCMAKE_BUILD_TYPE=<Debug|Release>)")
endif()

# include directories
include_directories(
	src
	ext/libuv)

# general compiler flags
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wno-long-long -Wextra -Werror -pedantic -std=c++11")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -O0 -g2")
if (CMAKE_CXX_COMPILER_ID MATCHES "Clang")
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fdiagnostics-color")
	set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3")
else()
	set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O4")
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

# login server
set(LOGIN_SRC
	src/login/server.cxx)

# character server
set(CHAR_SRC
	src/character/server.cxx)

# world server
set(WORLD_SRC
	src/world/server.cxx)

# common sources
set(COMMON_SRC
	${LOGIN_SRC}
	${CHAR_SRC}
	${WORLD_SRC}
	src/common/daemon.cxx
	src/common/log.cxx)

# seven hearts
set(SEVEN_HEARTS_SRC
	${COMMON_SRC}
	src/cli/main.cxx)

# seven hearts executable
add_executable(${CMAKE_PROJECT_NAME} ${SEVEN_HEARTS_SRC})

# linkage
target_link_libraries(${CMAKE_PROJECT_NAME} uv)
