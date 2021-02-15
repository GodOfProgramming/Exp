#pragma once

#include "exp/constants.hpp"
#include "exp/gl/enums.hpp"
#include "exp/gl/program.hpp"
#include "exp/gl/shader.hpp"
#include "exp/io/file.hpp"
#include "iresource.hpp"
#include "shader_meta.hpp"
#include "shader_program_meta.hpp"

namespace Exp
{
  namespace R
  {
    class Shaders: public IResource
    {
      Shaders() = default;

      using ShaderMap   = std::map<std::string, std::shared_ptr<GL::Shader>>;
      using ProgramMap  = std::map<std::string, std::shared_ptr<GL::Program>>;
      using ConfigCache = std::map<std::string, ShaderProgramMeta>;

     public:
      Shaders(const Shaders&) = delete;
      Shaders(Shaders&&)      = delete;
      auto operator=(const Shaders&) -> Shaders& = delete;
      auto operator=(Shaders&&) -> Shaders& = delete;

      static auto instance() -> Shaders&;

      void load_all();

      void release() final;

      auto load_program(std::string id) -> bool;

      void reload_program(std::string id);

      template <GL::Shader::Type T>
      auto load_shader(ShaderMeta& meta) -> bool;

      template <GL::Shader::Type T>
      auto reload_shader(ShaderMeta& meta) -> bool;

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

      auto load_shader_source(std::string_view filename) -> std::optional<std::string>;
    };

    template <GL::Shader::Type T>
    auto Shaders::load_shader(ShaderMeta& meta) -> bool
    {
      if (this->shader_map.find(meta.file) != this->shader_map.end()) {
        LOG(INFO) << "skipping shader load, already loaded shader " << meta.file;
        return true;
      }

      if (!meta.present) {
        LOG(ERROR) << "tried loading absent shader";
        return false;
      }

      LOG(INFO) << "loading shader " << meta.file;

      auto shader = std::make_shared<GL::Shader>();

      if (!shader->compile(T, meta.source, meta.error)) {
        LOG(ERROR) << "unable to compile shader:\n" << meta.error;
        return false;
      }

      this->shader_map[meta.file] = shader;

      return true;
    }

    template <GL::Shader::Type T>
    auto Shaders::reload_shader(ShaderMeta& meta) -> bool
    {
      LOG(INFO) << "reloading shader " << meta.file;

      auto abs_path = std::string(CFG_DIR_SHADERS) + "/" + meta.file;
      auto src_res  = IO::File::load(abs_path);
      if (!src_res) {
        LOG(ERROR) << "unable to load shader";
        return false;
      }

      auto src_file = src_res.ok_val();
      meta.source   = src_file.data;

      auto shader = std::make_shared<GL::Shader>();

      if (!shader->compile(T, meta.source, meta.error)) {
        LOG(ERROR) << "unable to compile shader:\n" << meta.error;
        return false;
      }

      this->shader_map[meta.file] = shader;

      return true;
    }
  }  // namespace R
}  // namespace Exp