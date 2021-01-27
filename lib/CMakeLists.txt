add_subdirectory(googletest)
add_subdirectory(glfw)
add_subdirectory(lua-cmake)

set(INCLUDES
  "${CMAKE_BINARY_DIR}/lib/lua-prefix/src/lua/src"
  "${CMAKE_CURRENT_SOURCE_DIR}/glfw/include"
  "${CMAKE_CURRENT_SOURCE_DIR}/sol2/include"
  "${CMAKE_CURRENT_SOURCE_DIR}/googletest/googletest/include"
  "${CMAKE_CURRENT_SOURCE_DIR}/googletest/googlemock/include"
)

set(LIBS
  lua::lib
  glfw
)

set(TEST_LIBS
  ${LIBS}
  gtest
  gmock
)

target_include_directories(${EXE} PUBLIC ${INCLUDES})
target_include_directories(${EXE_TEST} PUBLIC ${INCLUDES})

target_link_libraries(${EXE} ${LIBS})
target_link_libraries(${EXE_TEST} ${TEST_LIBS})
