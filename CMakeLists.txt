cmake_minimum_required(VERSION 3.16)

project(Exp VERSION 0.0.1)

# set compiler options

set(CMAKE_CXX_STANDARD 20)

set(CMAKE_CXX_STANDARD_REQUIRED True)

set(SHARED_COMPILE_OPTS -Wall -Wextra )# -pedantic -Werror)

# configure variables

set(EXE Exp)

set(EXE_TEST ExpTest)

set(SRC src)

add_executable(${EXE} "${SRC}/main.cpp")

add_executable(${EXE_TEST} "${SRC}/main.test.cpp")

target_compile_options(${EXE} PUBLIC ${SHARED_COMPILE_OPTS} -O3)

target_compile_options(${EXE_TEST} PUBLIC ${SHARED_COMPILE_OPTS} -g -O0 --coverage -fprofile-arcs -ftest-coverage)

# add sources

add_subdirectory(assets)

add_subdirectory(lib)

add_subdirectory(src)

target_link_libraries(${EXE} pthread)

target_link_libraries(${EXE_TEST} gcov pthread)

target_include_directories(${EXE} PUBLIC "${CMAKE_SOURCE_DIR}/src")

target_include_directories(${EXE_TEST} PUBLIC "${CMAKE_SOURCE_DIR}/src")

add_dependencies(${EXE} SHADERS)

add_dependencies(${EXE_TEST} SHADERS)
