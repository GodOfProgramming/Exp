add_subdirectory(exp)
add_subdirectory(test)

set(PCH_HEADERS
  "<glad/glad.h>"
  "<GLFW/glfw3.h>"
  "<imgui.h>"
  "<glm/glm.hpp>"
  "<glm/gtc/matrix_transform.hpp>"
  "<glm/gtc/type_ptr.hpp>"
  "<glm/gtc/quaternion.hpp>"
  "<glm/gtc/noise.hpp>"
  "<sol/sol.hpp>"
  "<nlohmann/json.hpp>"
  "<assimp/Importer.hpp>"
  "<assimp/scene.h>"
  "<assimp/postprocess.h>"
  "<filesystem>"
  "<fstream>"
  "<fstream>"
  "<iostream>"
  "<sstream>"
  "<string>"
  "<string_view>"
  "<variant>"
)

target_precompile_headers(${EXE} PUBLIC ${PCH_HEADERS})
target_precompile_headers(${EXE_TEST} PUBLIC ${PCH_HEADERS})
