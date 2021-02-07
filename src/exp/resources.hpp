#pragma once

#include "constants.hpp"
#include "gl.hpp"
#include "io.hpp"

namespace ExpGame
{
  namespace Resources
  {
    class Shaders
    {
      struct ConfigCacheEntry
      {
        bool has_vertex   = false;
        bool has_fragment = false;
        std::vector<std::string> shaders;
      };

      Shaders();

      using ShaderMap   = std::map<std::string, GL::Shader>;
      using ProgramMap  = std::map<std::string, GL::Program>;
      using ConfigCache = std::map<std::string, ConfigCacheEntry>;

     public:
      Shaders(const Shaders&) = delete;
      Shaders(Shaders&&)      = delete;
      auto operator=(const Shaders&) -> Shaders& = delete;
      auto operator=(Shaders&&) -> Shaders& = delete;

      static auto instance() -> Shaders&;

      void load_all();

      void release();

      template <GL::Shader::Type T>
      auto load_shader(std::string file) -> bool
      {
        if (this->shader_map.find(file) != this->shader_map.end()) {
          LOG(INFO) << "skipping shader load, already loaded shader " << file;
          return true;
        }

        LOG(INFO) << "loading shader " << file;

        auto abs_path = std::string(SHADER_DIR) + "/" + file;
        auto src_res  = IO::File::load(abs_path);
        if (!src_res) {
          LOG(ERROR) << "unable to load shader";
          return false;
        }

        auto src_file = src_res.ok_val();

        auto& shader = this->shader_map[file];
        if (!shader.compile(T, src_file.data)) {
          LOG(ERROR) << "unable to compile shader";
        }

        return true;
      }

      auto load_program(std::string id) -> bool;

      auto find_shader(std::string name) const -> ShaderMap::const_iterator;
      auto shader_begin() const -> ShaderMap::const_iterator;
      auto shader_end() const -> ShaderMap::const_iterator;

      auto find_program(std::string name) const -> ProgramMap::const_iterator;
      auto program_begin() const -> ProgramMap::const_iterator;
      auto program_end() const -> ProgramMap::const_iterator;

      auto cache_begin() const -> ConfigCache::const_iterator;
      auto cache_end() const -> ConfigCache::const_iterator;

     private:
      ShaderMap shader_map;
      ProgramMap program_map;
      ConfigCache cache;
    };
  }  // namespace Resources
}  // namespace ExpGame
