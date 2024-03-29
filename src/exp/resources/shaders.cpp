#include "shaders.hpp"

#include "exp/constants.hpp"
#include "exp/io.hpp"

namespace Exp
{
  namespace R
  {
    void Shaders::load_all(World& world)
    {
      LOG(INFO) << "loading shaders";

      std::set<std::string> valid_programs;
      std::set<std::string> attempted_loads;

      IO::iterate_dir_with_namespace(Cfg::Dir::SHADERS, "exp", [&](std::filesystem::path path, std::string nspace) {
        using nlohmann::json;
        IResource::load_json_file(path, [&](const json& objects) {
          if (!objects.is_object()) {
            LOG(WARNING) << "shader json is not in proper format, first type is not object";
            return;
          }

          for (const auto& shader_item : objects.items()) {
            auto id = nspace + "." + shader_item.key();

            LOG(INFO) << "loading shader " << id;

            if (attempted_loads.find(id) != attempted_loads.end()) {
              LOG(WARNING) << "already loaded shader program: " << id << ", skipping...";
              continue;
            }

            attempted_loads.emplace(id);

            auto shader_json = shader_item.value();
            ShaderProgramMeta shader_program(id, shader_json);

            if (!shader_program.is_valid()) {
              LOG(WARNING) << "shader " << id << " is not valid";
              continue;
            }

            bool should_continue = false;

            if (!this->load_shader<GL::Shader::Type::VERTEX>(shader_program.vertex)) {
              should_continue = true;
            }

            if (!this->load_shader<GL::Shader::Type::FRAGMENT>(shader_program.fragment)) {
              should_continue = true;
            }

            this->cache[id] = shader_program;

            if (should_continue) {
              continue;
            }

            valid_programs.emplace(id);
          }
        });
      });

      for (const auto& id : valid_programs) { this->load_program(id); }
    }

    void Shaders::reload_program(std::string id)
    {
      LOG(INFO) << "reloading program " << id;
      const auto iter = this->cache.find(id);
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

      if (!program->attach(*this->shader_map[cache_entry.vertex.file])) {
        LOG(ERROR) << "unable to attach vertex shader to program";
        return;
      }

      if (!program->attach(*this->shader_map[cache_entry.fragment.file])) {
        LOG(ERROR) << "unable to attach fragment shader to program";
        return;
      }

      if (!program->link(cache_entry.link_error)) {
        LOG(WARNING) << "unable to link program:\n" << cache_entry.link_error;
        return;
      }

      this->program_map[id] = program;
    }

    void Shaders::release()
    {
      this->shader_map.clear();
      this->program_map.clear();
      this->cache.clear();
    }

    auto Shaders::load_program(std::string id) -> bool
    {
      auto& cache_entry = this->cache[id];

      auto program          = std::make_shared<GL::Program>();
      this->program_map[id] = program;

      if (!program->attach(*this->shader_map[cache_entry.vertex.file])) {
        LOG(ERROR) << "unable to attach vertex shader to program";
        return false;
      }

      if (!program->attach(*this->shader_map[cache_entry.fragment.file])) {
        LOG(ERROR) << "unable to attach fragment shader to program";
        return false;
      }

      if (!program->link(cache_entry.link_error)) {
        LOG(WARNING) << "unable to link program:\n" << cache_entry.link_error;
        return false;
      }

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
  }  // namespace R
}  // namespace Exp