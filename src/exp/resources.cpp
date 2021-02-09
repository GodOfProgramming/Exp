#include "resources.hpp"

#include "constants.hpp"

namespace ExpGame
{
  namespace Resources
  {
    auto Shaders::instance() -> Shaders&
    {
      static Shaders shaders;
      return shaders;
    }

    void Shaders::load_all(const json& cfg)
    {
      LOG(INFO) << "loading shaders";

      if (!cfg.is_object()) {
        LOG(FATAL) << "shader json is not in proper format, first type is not object";
      }

      std::set<std::string> valid_programs;
      std::set<std::string> attempted_loads;

      for (const auto& shader_item : cfg.items()) {
        auto id = shader_item.key();

        if (attempted_loads.find(id) != attempted_loads.end()) {
          LOG(WARNING) << "already loaded shader program: " << id << ", skipping...";
          continue;
        }

        attempted_loads.emplace(id);

        auto shader_json = shader_item.value();
        const ShaderMeta shader(id, shader_json);

        if (!shader.is_valid()) {
          LOG(WARNING) << "shader " << id << " is not valid";
          continue;
        }

        bool should_continue = false;

        if (!this->load_shader<GL::Shader::Type::VERTEX>(shader.vertex)) {
          should_continue = true;
        }

        if (!this->load_shader<GL::Shader::Type::FRAGMENT>(shader.fragment)) {
          should_continue = true;
        }

        this->cache[id] = shader;

        if (should_continue) {
          continue;
        }

        valid_programs.emplace(id);
      }

      for (const auto& id : valid_programs) { this->load_program(id); }

      DLOG(INFO) << "loaded shaders";
    }

    void Shaders::reload_program(std::string id)
    {
      LOG(INFO) << "reloading program " << id;
      auto iter = this->cache.find(id);
      if (iter == this->cache.end()) {
        LOG(WARNING) << "unable to reload program, does not exist";
        return;
      }

      auto& cache_entry = iter->second;

      if (!this->reload_shader<GL::Shader::Type::VERTEX>(cache_entry.vertex)) {
        LOG(WARNING) << "unable to reload vertex shader";
        return;
      }

      if (!this->reload_shader<GL::Shader::Type::FRAGMENT>(cache_entry.fragment)) {
        LOG(WARNING) << "unable to reload fragment shader";
        return;
      }

      auto program = std::make_shared<GL::Program>();

      if (!program->attach(*this->shader_map[cache_entry.vertex])) {
        LOG(ERROR) << "unable to attach vertex shader to program";
        return;
      }

      if (!program->attach(*this->shader_map[cache_entry.fragment])) {
        LOG(ERROR) << "unable to attach fragment shader to program";
        return;
      }

      if (!program->link()) {
        LOG(WARNING) << "unable to link program";
        return;
      }

      this->program_map[id] = program;

      DLOG(INFO) << "reloaded program";
    }

    void Shaders::release()
    {
      this->shader_map.clear();
      this->program_map.clear();
      this->cache.clear();
    }

    auto Shaders::load_program(std::string id) -> bool
    {
      LOG(INFO) << "loading program: " << id;

      const auto& cache_entry = this->cache[id];

      auto program          = std::make_shared<GL::Program>();
      this->program_map[id] = program;

      if (!program->attach(*this->shader_map[cache_entry.vertex])) {
        LOG(ERROR) << "unable to attach vertex shader to program";
        return false;
      }

      if (!program->attach(*this->shader_map[cache_entry.fragment])) {
        LOG(ERROR) << "unable to attach fragment shader to program";
        return false;
      }

      if (!program->link()) {
        LOG(WARNING) << "unable to link program";
      }

      DLOG(INFO) << "loaded program";

      return true;
    }

    auto Shaders::find_shader(std::string id) const -> ShaderMap::const_iterator
    {
      return this->shader_map.find(id);
    }

    auto Shaders::shader_begin() const -> ShaderMap::const_iterator
    {
      return this->shader_map.begin();
    }

    auto Shaders::shader_end() const -> ShaderMap::const_iterator
    {
      return this->shader_map.end();
    }

    auto Shaders::find_program(std::string id) const -> ProgramMap::const_iterator
    {
      return this->program_map.find(id);
    }

    auto Shaders::program_begin() const -> ProgramMap::const_iterator
    {
      return this->program_map.begin();
    }

    auto Shaders::program_end() const -> ProgramMap::const_iterator
    {
      return this->program_map.end();
    }

    auto Shaders::cache_begin() const -> ConfigCache::const_iterator
    {
      return this->cache.begin();
    }

    auto Shaders::cache_end() const -> ConfigCache::const_iterator
    {
      return this->cache.end();
    }

    Shaders::ShaderMeta::ShaderMeta(std::string st, json& sj)
     : shader_type(st)
    {
      if (!sj.is_object()) {
        LOG(WARNING) << "shader is not object, skipping: " << sj.dump(2);
        return;
      }

      for (const auto& item : sj.items()) {
        auto shader_type    = item.key();
        auto filename_value = item.value();

        if (!filename_value.is_string()) {
          LOG(INFO) << "could not load shader: " << filename_value.dump(2);
          continue;
        }

        if (shader_type == SHADER_VERTEX_KEY) {
          if (this->has_vertex()) {
            LOG(WARNING) << "program already has vertex shader linked: " << this->shader_type;
            continue;
          }

          this->vertex = filename_value;
        } else if (shader_type == SHADER_FRAGMENT_KEY) {
          if (this->has_fragment()) {
            LOG(WARNING) << "program already has fragment shader linked: " << this->shader_type;
            continue;
          }

          this->fragment = filename_value;
        } else {
          LOG(WARNING) << "unsupported shader type " << shader_type << ", not loading";
        }
      }
    }

    auto Shaders::ShaderMeta::is_valid() const noexcept -> bool
    {
      return !this->shader_type.empty() && this->has_vertex() && this->has_fragment();
    }

    auto Shaders::ShaderMeta::has_vertex() const noexcept -> bool
    {
      return !this->vertex.empty();
    }

    auto Shaders::ShaderMeta::has_fragment() const noexcept -> bool
    {
      return !this->fragment.empty();
    }
  }  // namespace Resources
}  // namespace ExpGame