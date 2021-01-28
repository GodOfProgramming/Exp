add_subdirectory(glad)
add_subdirectory(glfw)
add_subdirectory(imgui)
add_subdirectory(lua)
add_subdirectory(sol)
add_subdirectory(gtest)

set(LIBS
  glad
  glfw
  imgui::lib
  lua::lib
  sol2::sol2
)

set(TEST_LIBS
  ${LIBS}
  gtest
  gmock
)

target_link_libraries(${EXE} ${LIBS})
target_link_libraries(${EXE_TEST} ${TEST_LIBS})
