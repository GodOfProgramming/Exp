file(GLOB SRC_FILES CONFIGURE_DEPENDS "*.cpp")

target_sources(${EXE} PUBLIC ${SRC_FILES})
target_sources(${EXE_TEST} PUBLIC ${SRC_FILES})
