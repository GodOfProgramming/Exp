cmake_minimum_required(VERSION 3.16)
project(lua VERSION 5.2.2 DESCRIPTION "Lua CMake Support" LANGUAGES C)

set(SRC_DIR upstream)

file(GLOB SRC_FILES CONFIGURE_DEPENDS "${SRC_DIR}/*.c")

add_library(lua-lib ${SRC_FILES})

add_library(lua-include INTERFACE)

target_include_directories(lua-include INTERFACE ${SRC_DIR})

target_link_libraries(lua-lib INTERFACE lua-include)

add_library(lua::lib ALIAS lua-lib)

add_library(lua::include ALIAS lua-include)
