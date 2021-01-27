add_subdirectory(googletest)
add_subdirectory(glfw)
add_subdirectory(lua)
add_subdirectory(sol)
add_subdirectory(imgui)

set(LIBS
  glfw
  lua::lib
  sol2::sol2
  imgui::lib
)

set(TEST_LIBS
  ${LIBS}
  gtest
  gmock
)

target_link_libraries(${EXE} ${LIBS})
target_link_libraries(${EXE_TEST} ${TEST_LIBS})
