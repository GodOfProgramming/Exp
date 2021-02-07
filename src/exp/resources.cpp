#include "resources.hpp"

#include "constants.hpp"

namespace ExpGame
{
  namespace Resources
  {
    Shaders::Shaders() {}

    auto Shaders::instance() -> Shaders&
    {
      static Shaders shaders;
      return shaders;
    }

    void Shaders::load_all()
    {
      using nlohmann::json;

      LOG(INFO) << "loading shaders";

      auto file_res = IO::File::load(SHADER_CFG_FILE);
      if (!file_res) {
        LOG(FATAL) << "unable to load shader configuration file: " << file_res.err_val();
        return;
      }

      auto file = file_res.ok_val();

      json j;

      try {
        j = json::parse(file.data);
      } catch (std::exception& e) {
        LOG(FATAL) << "could not parse json: " << e.what();
      }

      if (!j.is_object()) {
        LOG(FATAL) << "shader json is not in proper format, first type is not object";
      }

      std::set<std::string> program_ids;

      for (const auto& shader_json : j.items()) {
        auto id = shader_json.key();
        if (program_ids.find(id) != program_ids.end()) {
          LOG(WARNING) << "already loaded shader program: " << id << ", skipping...";
          continue;
        }

        auto shader = shader_json.value();

        if (!shader.is_object()) {
          LOG(WARNING) << "shader is not object, skipping: " << shader.dump(2);
          continue;
        }

        auto& cfg = this->cache[id];

        for (const auto& item : shader.items()) {
          auto shader_type    = item.key();
          auto filename_value = item.value();
          if (!filename_value.is_string()) {
            LOG(INFO) << "could not load shader: " << filename_value.dump(2);
          }

          std::string filename = filename_value;

          // handle shader types
          // currently supported: vertex, fragment
          if (shader_type == "vertex") {
            if (cfg.has_vertex) {
              LOG(WARNING) << "program already has vertex shader linked: " << id;
              continue;
            }

            if (this->load_shader<GL::Shader::Type::VERTEX>(filename)) {
              cfg.has_vertex = true;
              cfg.shaders.push_back(filename);
            }
          } else if (shader_type == "fragment") {
            if (cfg.has_fragment) {
              LOG(WARNING) << "program already has fragment shader linked: " << id;
              continue;
            }

            if (this->load_shader<GL::Shader::Type::FRAGMENT>(filename)) {
              cfg.has_fragment = true;
              cfg.shaders.push_back(filename);
            }
          } else {
            LOG(WARNING) << "unsupported shader type " << shader_type << ", not loading";
          }
        }

        program_ids.emplace(id);
      }

      for (const auto& id : program_ids) { this->load_program(id); }
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
      const auto iter = this->cache.find(id);

      // sanity check
      if (iter == this->cache.end()) {
        LOG(ERROR) << "unable to load program " << id << " not found";
        return false;
      }

      // validate shaders were loaded
      const auto& cfg = iter->second;

      if (!cfg.has_vertex) {
        LOG(ERROR) << "unable to attach shader program " << id << ", vertex not present";
        return false;
      }

      if (!cfg.has_vertex) {
        LOG(ERROR) << "unable to attach shader program " << id << ", vertex not present";
        return false;
      }

      for (const auto& shader : cfg.shaders) {
        auto shader_iter = this->shader_map.find(shader);
        if (shader_iter == this->shader_map.end()) {
          LOG(ERROR) << "unable to attach shader program " << id << ", shader not loaded";
          return false;
        }

        if (!shader_iter->second.is_valid()) {
          LOG(ERROR) << "tried to load invalid shader for program " << id;
          return false;
        }
      }

      // if everything passes, attach them

      auto& program = this->program_map[id];

      for (const auto& shader : cfg.shaders) {
        if (!program.attach(this->shader_map[shader])) {
          LOG(ERROR) << "unable to attach shaders to program";
          return false;
        }
      }

      if (!program.link()) {
        LOG(WARNING) << "unable to link program";
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
  }  // namespace Resources
}  // namespace ExpGame