cmake_minimum_required(VERSION 3.16)
project(imgui VERSION 1.8 DESCRIPTION "ImGui CMake Support" LANGUAGES CXX)

set(SRC_DIR upstream)
set(BACKEND_DIR "${SRC_DIR}/backends")

file(GLOB ROOT_SRC_FILES CONFIGURE_DEPENDS "${SRC_DIR}/*.cpp")

set(SRC_FILES
  ${ROOT_SRC_FILES}
  "${BACKEND_DIR}/imgui_impl_opengl3.cpp"
  "${BACKEND_DIR}/imgui_impl_glfw.cpp"
)

add_library(imgui-lib ${SRC_FILES})

add_library(imgui-include INTERFACE)

target_include_directories(imgui-include INTERFACE ${SRC_DIR})

target_link_libraries(imgui-lib INTERFACE imgui-include)

add_library(imgui::lib ALIAS imgui-lib)

add_library(imgui::include ALIAS imgui-include)
