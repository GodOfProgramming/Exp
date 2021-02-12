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
        ShaderProgramMeta shader(id, shader_json);

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

      if (!program->attach(*this->shader_map[cache_entry.vertex.file])) {
        LOG(ERROR) << "unable to attach vertex shader to program";
        return;
      }

      if (!program->attach(*this->shader_map[cache_entry.fragment.file])) {
        LOG(ERROR) << "unable to attach fragment shader to program";
        return;
      }

      if (!program->link(cache_entry.link_error)) {
        LOG(WARNING) << "unable to link program";
        return;
      }

      this->program_map[id] = program;

      DLOG(INFO) << "reloaded program";
    }

    void Shaders::release()
    {
      DLOG(INFO) << "releasing shaders";
      this->shader_map.clear();
      this->program_map.clear();
      this->cache.clear();
    }

    auto Shaders::load_program(std::string id) -> bool
    {
      LOG(INFO) << "loading program: " << id;

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
        LOG(WARNING) << "unable to link program";
        return false;
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

    ShaderProgramMeta::ShaderProgramMeta(std::string st, json& sj)
     : type(st)
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

        ShaderMeta* shader = nullptr;

        if (shader_type == SHADER_VERTEX_KEY) {
          if (!this->vertex.file.empty()) {
            LOG(WARNING) << "program already has vertex shader linked: " << this->type;
            continue;
          }

          shader = &this->vertex;
        } else if (shader_type == SHADER_FRAGMENT_KEY) {
          if (!this->fragment.file.empty()) {
            LOG(WARNING) << "program already has fragment shader linked: " << this->type;
            continue;
          }

          shader = &this->fragment;
        } else {
          LOG(WARNING) << "unsupported shader type " << shader_type << ", not loading";
          continue;
        }

        shader->file  = filename_value;
        auto abs_path = std::string(SHADER_DIR) + "/" + shader->file;
        auto src_res  = IO::File::load(abs_path);
        if (!src_res) {
          LOG(ERROR) << "unable to load shader";
          continue;
        }

        auto src_file   = src_res.ok_val();
        shader->source  = src_file.data;
        shader->present = true;
      }
    }

    auto ShaderProgramMeta::is_valid() const noexcept -> bool
    {
      return !this->type.empty() && this->has_vertex() && this->has_fragment();
    }

    auto ShaderProgramMeta::has_vertex() const noexcept -> bool
    {
      return this->vertex.present;
    }

    auto ShaderProgramMeta::has_fragment() const noexcept -> bool
    {
      return this->fragment.present;
    }

    auto GameObjects::instance() noexcept -> GameObjects&
    {
      static GameObjects objs;
      return objs;
    }

    void GameObjects::load_all()
    {
      auto& shaders = Shaders::instance();
      using nlohmann::json;
      std::function<void(std::filesystem::path, std::string)> decend = [&](std::filesystem::path dir, std::string nspace) {
        for (const auto& iter : std::filesystem::directory_iterator(dir)) {
          if (iter.is_directory()) {
            decend(iter, nspace + "." + iter.path().filename().string());
          } else if (iter.is_regular_file()) {
            nspace += "." + iter.path().stem().string();

            LOG(INFO) << "loading game objects under namespace " << nspace;

            auto file_res = IO::File::load(iter);
            if (!file_res) {
              LOG(WARNING) << "unable to load game object configuration file: " << file_res.err_val();
            }

            auto file = file_res.ok_val();

            json objects;

            try {
              objects = json::parse(file.data);
            } catch (std::exception& e) {
              LOG(WARNING) << "could not parse json: " << e.what();
              continue;
            }

            for (const auto item : objects.items()) {
              auto id  = nspace + "." + item.key();
              auto obj = item.value();

              std::string shader_id = obj["shader"];
              auto shader_iter      = shaders.find_program(shader_id);
              if (shader_iter == shaders.program_end()) {
                LOG(WARNING) << "cannot find shader with id " << shader_id;
                continue;
              } else {
                DLOG(INFO) << "object has shader id " << shader_id;
              }

              auto shader = shader_iter->second;

              if (!shader->is_valid()) {
                LOG(WARNING) << "cannot load item, shader " << shader_id << " not valid";
                continue;
              }

              auto vao = std::make_shared<GL::VAO>();

              if (!vao->gen()) {
                LOG(WARNING) << "unable to generate vao";
                continue;
              } else {
                DLOG(INFO) << "generated vao";
              }

              if (!vao->valid()) {
                LOG(WARNING) << "unable to load object, vao not valid";
                continue;
              }

              auto vbo = std::make_shared<GL::VBO>(vao);

              if (!vbo->gen()) {
                LOG(WARNING) << "could not generate vbo";
                continue;
              } else {
                DLOG(INFO) << "generated vbo";
              }

              if (!vbo->valid()) {
                LOG(WARNING) << "unable to load object, vbo not valid";
                continue;
              }

              if (obj["vertices"].is_array()) {
                std::vector<float> list = obj["vertices"];
                if (list.size() % 3 != 0) {
                  LOG(WARNING) << "cannot load item " << id << ", invalid number of vertices";
                  continue;
                }
                std::vector<glm::vec3> vertices(list.size() / 3);

                for (std::size_t i = 0; i < list.size(); i += 3) {
                  glm::vec3 vertex{};
                  vertex.x        = list[i + 0];
                  vertex.y        = list[i + 1];
                  vertex.z        = list[i + 2];
                  vertices[i / 3] = vertex;
                  DLOG(INFO) << "adding vertex (" << vertex.x << ", " << vertex.y << ", " << vertex.z << ") at index " << i / 3;
                }

                if (!vbo->set<GL::GlDraw::STATIC>(vertices)) {
                  LOG(WARNING) << "unable to set vertices to object";
                  continue;
                }
              } else {
                LOG(WARNING) << "no way to create object vertices";
                continue;
              }

              ObjectMeta meta;
              meta.shader = shader;
              meta.vao    = vao;
              meta.vbo    = vbo;

              DLOG(INFO) << "adding game object " << id;
              this->objects.emplace(id, meta);
            }
          }
        }
      };

      decend(GAME_OBJECT_DIR, std::string{ "exp" });
    }

    void GameObjects::release()
    {
      DLOG(INFO) << "releasing game objects";
      this->objects.clear();
    }

    auto GameObjects::find(std::string id) const noexcept -> ObjectMap::const_iterator
    {
      return this->objects.find(id);
    }

    auto GameObjects::begin() const noexcept -> ObjectMap::const_iterator
    {
      return this->objects.begin();
    }

    auto GameObjects::end() const noexcept -> ObjectMap::const_iterator
    {
      return this->objects.end();
    }
  }  // namespace Resources
}  // namespace ExpGame