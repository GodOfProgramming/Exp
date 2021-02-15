file(GLOB SHADER_SRC CONFIGURE_DEPENDS "${CMAKE_CURRENT_LIST_DIR}/shaders/src/*.glsl")

foreach(SRC IN ITEMS ${SHADER_SRC})
  string(REGEX REPLACE "${CMAKE_CURRENT_LIST_DIR}/shaders/src" "${CMAKE_CURRENT_LIST_DIR}/shaders/out" OUT ${SRC})
  list(APPEND SHADER_OUT ${OUT})
endforeach()

add_custom_command(
  OUTPUT ${SHADER_OUT}
  COMMAND "${CMAKE_CURRENT_LIST_DIR}/shader.rb"
  DEPENDS ${SHADER_SRC} "${CMAKE_CURRENT_LIST_DIR}/shader.rb"
)

add_custom_target(shaders DEPENDS ${SHADER_OUT})