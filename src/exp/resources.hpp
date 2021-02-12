#pragma once

#include "constants.hpp"
#include "gl.hpp"
#include "io.hpp"

namespace ExpGame
{
  namespace Resources
  {
    class IResource
    {
     public:
      virtual void release() = 0;
    };

    class ShaderMeta
    {
      using json = nlohmann::json;

     public:
      ShaderMeta() = default;
      ShaderMeta(std::string st, json& sj);

      auto is_valid() const noexcept -> bool;

      auto has_vertex() const noexcept -> bool;

      auto has_fragment() const noexcept -> bool;

      std::string shader_type;
      std::string vertex;
      std::string fragment;
    };

    class Shaders: public IResource
    {
      Shaders() = default;

      using ShaderMap   = std::map<std::string, std::shared_ptr<GL::Shader>>;
      using ProgramMap  = std::map<std::string, std::shared_ptr<GL::Program>>;
      using ConfigCache = std::map<std::string, ShaderMeta>;
      using json        = nlohmann::json;

     public:
      Shaders(const Shaders&) = delete;
      Shaders(Shaders&&)      = delete;
      auto operator=(const Shaders&) -> Shaders& = delete;
      auto operator=(Shaders&&) -> Shaders& = delete;

      static auto instance() -> Shaders&;

      void load_all(const json& cfg);

      void release() final;

      auto load_program(std::string id) -> bool;

      void reload_program(std::string id);

      template <GL::Shader::Type T>
      auto load_shader(std::string file) -> bool;

      template <GL::Shader::Type T>
      auto reload_shader(std::string file) -> bool;

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
    auto Shaders::load_shader(std::string file) -> bool
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

      auto shader            = std::make_shared<GL::Shader>();
      this->shader_map[file] = shader;

      if (!shader->compile(T, src_file.data)) {
        LOG(ERROR) << "unable to compile shader:\n" << shader->error();
        return false;
      }

      DLOG(INFO) << "loaded shader";

      return true;
    }

    template <GL::Shader::Type T>
    auto Shaders::reload_shader(std::string file) -> bool
    {
      LOG(INFO) << "reloading shader " << file;

      auto abs_path = std::string(SHADER_DIR) + "/" + file;
      auto src_res  = IO::File::load(abs_path);
      if (!src_res) {
        LOG(ERROR) << "unable to load shader";
        return false;
      }

      auto src_file = src_res.ok_val();

      auto shader = std::make_shared<GL::Shader>();

      if (!shader->compile(T, src_file.data)) {
        LOG(ERROR) << "unable to compile shader:\n" << shader->error();
        return false;
      }

      DLOG(INFO) << "reloaded shader";

      this->shader_map[file] = shader;

      return true;
    }

    struct ObjectMeta
    {
      std::shared_ptr<GL::Program> shader;
      std::shared_ptr<GL::VAO> vao;
      std::shared_ptr<GL::VBO> vbo;
    };

    class GameObjects: public IResource
    {
      using ObjectMap = std::map<std::string, ObjectMeta>;
      GameObjects()   = default;

     public:
      GameObjects(const GameObjects&) = delete;
      GameObjects(GameObjects&&)      = delete;
      auto operator=(const GameObjects&) -> GameObjects& = delete;
      auto operator=(GameObjects&&) -> GameObjects& = delete;

      static auto instance() noexcept -> GameObjects&;

      void load_all();

      void release() final;

      auto find(std::string id) const noexcept -> ObjectMap::const_iterator;
      auto begin() const noexcept -> ObjectMap::const_iterator;
      auto end() const noexcept -> ObjectMap::const_iterator;

      auto lookup(std::string id) const noexcept -> ObjectMap::value_type;

     private:
      ObjectMap objects;
    };
  }  // namespace Resources
}  // namespace ExpGame
