file(GLOB_RECURSE SRC_FILES "./*.cpp")

target_sources(${EXE_TEST} PUBLIC ${SRC_FILES})
