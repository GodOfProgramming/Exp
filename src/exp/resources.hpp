#pragma once

#include "gl.hpp"
#include "io.hpp"

namespace ExpGame
{
  namespace Resources
  {
    class Shaders
    {
      Shaders();

     public:
      Shaders(const Shaders&) = delete;
      Shaders(Shaders&&)      = delete;
      auto operator=(const Shaders&) -> Shaders& = delete;
      auto operator=(Shaders&&) -> Shaders& = delete;

      static auto instance() -> Shaders&;

      template <GL::Shader::Type T>
      auto load_shader(std::string id, std::string file) -> bool
      {
        if (this->shader_map.find(id) != this->shader_map.end()) {
          LOG(INFO) << "skipping shader load, already loaded shader id " << id;
          return false;
        } else {
          LOG(INFO) << "loading shader " << id << " (" << file << ')';
        }

        auto v_res = IO::File::load(file);
        if (v_res.is_err()) {
          LOG(ERROR) << "Unable to load shader";
          return false;
        }

        auto v_file = v_res.ok_val();

        auto shader = std::make_shared<GL::Shader>();
        if (!shader->compile(T, v_file.data)) {
          LOG(ERROR) << "Unable to compile shader";
          return false;
        }

        this->shader_map[id] = shader;
        return true;
      }

      auto load_program(std::string id, std::string vertex_id, std::string fragment_id) -> bool;

     private:
      std::map<std::string, std::shared_ptr<GL::Program>> program_map;
      std::map<std::string, std::shared_ptr<GL::Shader>> shader_map;
    };
  }  // namespace Resources
}  // namespace ExpGame
