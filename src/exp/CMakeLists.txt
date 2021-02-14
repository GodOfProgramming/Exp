file(GLOB_RECURSE SRC_FILES "./*.cpp")

target_sources(${EXE} PUBLIC ${SRC_FILES})
target_sources(${EXE_TEST} PUBLIC ${SRC_FILES})
