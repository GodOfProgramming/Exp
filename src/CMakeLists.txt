add_subdirectory(exp)
add_subdirectory(test)

set(PCH_HEADERS
  "<fmt/format.h>"
  "<glog/logging.h>"
  "<gflags/gflags.h>"
  "<glad/glad.h>"
  "<GLFW/glfw3.h>"
  "<imgui.h>"
  "<backends/imgui_impl_glfw.h>"
  "<backends/imgui_impl_opengl3.h>"
  "<glm/glm.hpp>"
  "<glm/gtc/matrix_transform.hpp>"
  "<glm/gtc/type_ptr.hpp>"
  "<glm/gtc/quaternion.hpp>"
  "<glm/gtc/noise.hpp>"
  "<sol/sol.hpp>"
  "<nlohmann/json.hpp>"
  "<tinyxml2.h>"
  "<assimp/Importer.hpp>"
  "<assimp/scene.h>"
  "<assimp/postprocess.h>"
  "<chrono>"
  "<condition_variable>"
  "<deque>"
  "<filesystem>"
  "<fstream>"
  "<fstream>"
  "<functional>"
  "<iostream>"
  "<map>"
  "<memory>"
  "<mutex>"
  "<queue>"
  "<regex>"
  "<set>"
  "<sstream>"
  "<string>"
  "<string_view>"
  "<thread>"
  "<type_traits>"
  "<unordered_map>"
  "<unordered_set>"
  "<variant>"
)

set(TEST_PCH_HEADERS
  "<gtest/gtest.h>"
)

target_precompile_headers(${EXE} PUBLIC ${PCH_HEADERS})
target_precompile_headers(${EXE_TEST} PUBLIC ${TEST_PCH_HEADERS} ${PCH_HEADERS})
