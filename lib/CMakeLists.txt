add_subdirectory(gtest)
add_subdirectory(fmt)
add_subdirectory(gflags)
add_subdirectory(glog)
add_subdirectory(glad)
add_subdirectory(glm)
add_subdirectory(glfw)
add_subdirectory(imgui)
add_subdirectory(lua)
add_subdirectory(sol)
add_subdirectory(assimp)
add_subdirectory(stb)
add_subdirectory(json)
add_subdirectory(toml)
add_subdirectory(xml)

set(LIBS
  fmt
  gflags
  glog
  glad
  glm
  glfw
  imgui::lib
  lua::lib
  sol2::sol2
  nlohmann_json
  tinyxml2
  tomlplusplus
  assimp
)

set(TEST_LIBS
  ${LIBS}
  gtest
  gmock
)

target_link_libraries(${EXE} ${LIBS})
target_link_libraries(${EXE_TEST} ${TEST_LIBS})
