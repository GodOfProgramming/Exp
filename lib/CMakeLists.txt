add_subdirectory(fmt)
add_subdirectory(gflags)
add_subdirectory(glog)
add_subdirectory(glad)
add_subdirectory(glm)
add_subdirectory(gli)
add_subdirectory(glfw)
add_subdirectory(imgui)
add_subdirectory(lua)
add_subdirectory(sol)
add_subdirectory(assimp)
add_subdirectory(json)
add_subdirectory(xml)
add_subdirectory(gtest)

set(LIBS
  fmt
  gflags
  glog
  glad
  glm
  gli
  glfw
  imgui::lib
  lua::lib
  sol2::sol2
  nlohmann_json
  tinyxml2
  assimp
)

set(TEST_LIBS
  ${LIBS}
  gtest
  gmock
)

target_link_libraries(${EXE} ${LIBS})
target_link_libraries(${EXE_TEST} ${TEST_LIBS})
