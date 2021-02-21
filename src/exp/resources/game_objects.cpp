#include "game_objects.hpp"

#include "exp/constants.hpp"
#include "exp/io.hpp"
#include "models.hpp"
#include "shaders.hpp"

namespace Exp
{
  namespace R
  {
    auto GameObjects::instance() noexcept -> GameObjects&
    {
      static GameObjects objs;
      return objs;
    }

    void GameObjects::add_usertype(sol::state_view state)
    {
      state.new_usertype<GameObjects>(
       Lua::Usertypes::R::GAME_OBJECTS, "instance", &GameObjects::instance, "keys", &GameObjects::keys, "get", &GameObjects::get);
    }

    void GameObjects::load_all()
    {
      LOG(INFO) << "loading game objects";
      auto& shaders = Shaders::instance();
      auto& models  = Models::instance();
      IO::iterate_dir_with_namespace(Cfg::Dir::GAME_OBJECTS, std::string{ "exp" }, [&](const std::filesystem::path path, const std::string& nspace) {
        this->load_json_file(path, [&](const json& objects) {
          if (!objects.is_object()) {
            LOG(WARNING) << "shader json is not in proper format, first type is not object";
            return;
          }

          for (const auto item : objects.items()) {
            auto id  = nspace + "." + item.key();
            auto obj = item.value();

            LOG(INFO) << "loading game object " << id;

            std::string shader_id = obj["shader"];
            auto shader_iter      = shaders.find_program(shader_id);
            if (shader_iter == shaders.program_end()) {
              LOG(WARNING) << "cannot find shader with id " << shader_id;
              continue;
            }

            auto shader = shader_iter->second;

            if (!shader->is_valid()) {
              LOG(WARNING) << "cannot load item, shader " << shader_id << " not valid";
              continue;
            }

            std::string model_id = obj["model"];
            auto model_iter      = models.find(model_id);
            if (model_iter == models.end()) {
              LOG(WARNING) << "cannot find model with id " << model_id;
              continue;
            }

            Render::DrawDescription desc;

            auto drawdesc_json = obj["draw_description"];
            if (drawdesc_json.is_object()) {
              if (!this->parse_drawdesc(drawdesc_json, desc)) {
                LOG(WARNING) << "unable to parse partial or full draw desc on object, using what was valid";
              }
            } else if (!drawdesc_json.is_null()) {
              LOG(WARNING) << "detected draw description in config but desc was not of type object";
              continue;
            }

            auto model = model_iter->second;

            ObjectMeta meta;
            meta.id       = id;
            meta.shader   = shader;
            meta.model    = model;
            meta.drawdesc = desc;

            auto script_json = obj["script"];
            if (script_json.is_string()) {
              meta.script_id = script_json;
            } else if (!script_json.is_null()) {
              LOG(WARNING) << "detected script in config but was not of type string";
              continue;
            }

            this->objects.emplace(id, meta);
          }
        });
      });
    }

    void GameObjects::release()
    {
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

    auto GameObjects::keys() const noexcept -> std::vector<ObjectMap::key_type>
    {
      std::vector<ObjectMap::key_type> k;
      std::transform(std::begin(this->objects), std::end(this->objects), std::back_inserter(k), [](const auto& kvp) { return kvp.first; });
      return k;
    }

    auto GameObjects::get(std::string id) const noexcept -> std::unique_ptr<ObjectMap::mapped_type>
    {
      auto iter = this->find(id);
      if (iter == this->end()) {
        return nullptr;
      } else {
        return std::make_unique<ObjectMap::mapped_type>(iter->second);
      }
    }

    auto GameObjects::parse_drawdesc(const json& desc_json, Render::DrawDescription& desc) -> bool
    {
      auto wireframe = desc_json["wireframe"];
      if (wireframe.is_boolean()) {
        desc.wireframe = wireframe;
      } else if (!wireframe.is_null()) {
        LOG(WARNING) << "detected wireframe option in draw desc but was not of boolean type: " << wireframe.dump();
        return false;
      }

      return true;
    }
  }  // namespace R
}  // namespace Exp
