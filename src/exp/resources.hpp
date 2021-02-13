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

    struct ShaderMeta
    {
      std::string file;
      std::string source;
      std::string error;
      bool present = false;
    };

    class ShaderProgramMeta
    {
      using json = nlohmann::json;

     public:
      ShaderProgramMeta() = default;
      ShaderProgramMeta(std::string st, json& sj);

      auto is_valid() const noexcept -> bool;

      auto has_vertex() const noexcept -> bool;

      auto has_fragment() const noexcept -> bool;

      std::string type;
      std::string link_error;
      ShaderMeta vertex;
      ShaderMeta fragment;
    };

    class Shaders: public IResource
    {
      Shaders() = default;

      using ShaderMap   = std::map<std::string, std::shared_ptr<GL::Shader>>;
      using ProgramMap  = std::map<std::string, std::shared_ptr<GL::Program>>;
      using ConfigCache = std::map<std::string, ShaderProgramMeta>;
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

      auto abs_path = std::string(SHADER_DIR) + "/" + meta.file;
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

    struct ModelMeta
    {
      std::shared_ptr<GL::VAO> vao;
      std::shared_ptr<GL::VBO> vbo;
      std::shared_ptr<GL::EBO> ebo;
    };

    class Models
    {
      Models() = default;

      using ModelMap = std::map<std::string, std::shared_ptr<ModelMeta>>;

     public:
      Models(const Models&) = delete;
      Models(Models&&)      = delete;
      auto operator=(const Models&) -> Models& = delete;
      auto operator=(Models&&) -> Models& = delete;

      static auto instance() noexcept -> Models&;

      void load_all();

      void release();

      auto find(std::string id) const noexcept -> ModelMap::const_iterator;
      auto begin() const noexcept -> ModelMap::const_iterator;
      auto end() const noexcept -> ModelMap::const_iterator;

     private:
      ModelMap models;
    };

    struct ObjectMeta
    {
      std::shared_ptr<GL::Program> shader;
      std::shared_ptr<ModelMeta> model;
      GL::DrawDescription drawdesc;
    };

    class GameObjects: public IResource
    {
      using ObjectMap = std::map<std::string, ObjectMeta>;
      GameObjects()   = default;

      using json = nlohmann::json;

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

     private:
      ObjectMap objects;

      auto parse_drawdesc(const json&, GL::DrawDescription& desc) -> bool;
    };
  }  // namespace Resources
}  // namespace ExpGame
