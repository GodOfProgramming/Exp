#include "game_objects.hpp"

#include "exp/io.hpp"
#include "models.hpp"
#include "shaders.hpp"

namespace Exp
{
  namespace Resources
  {
    auto GameObjects::instance() noexcept -> GameObjects&
    {
      static GameObjects objs;
      return objs;
    }

    void GameObjects::load_all()
    {
      auto& shaders = Shaders::instance();
      auto& models  = Models::instance();
      IO::iterate_dir_with_namespace(GAME_OBJECT_DIR, std::string{ "exp" }, [&](const std::filesystem::path path, const std::string& nspace) {
        auto file_res = IO::File::load(path);
        if (!file_res) {
          LOG(WARNING) << "unable to load game object configuration file: " << file_res.err_val();
          return;
        }

        auto file = file_res.ok_val();

        json objects;

        try {
          objects = json::parse(file.data);
        } catch (std::exception& e) {
          LOG(WARNING) << "could not parse json: " << e.what();
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

          GL::DrawDescription desc;

          auto drawdesc_json = obj["draw_description"];
          if (drawdesc_json.is_object()) {
            if (!this->parse_drawdesc(drawdesc_json, desc)) {
              LOG(WARNING) << "unable to parse partial or full draw desc on object, using what was valid";
            }
          } else if (!drawdesc_json.is_null()) {
            LOG(WARNING) << "detected draw description in config but desc was not of type object";
          }

          auto model = model_iter->second;

          ObjectMeta meta;
          meta.shader   = shader;
          meta.model    = model;
          meta.drawdesc = desc;

          this->objects.emplace(id, meta);
        }
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

    auto GameObjects::parse_drawdesc(const json& desc_json, GL::DrawDescription& desc) -> bool
    {
      auto wireframe = desc_json["wireframe"];
      if (wireframe.is_boolean()) {
        desc.wireframe = wireframe;
      } else if (!wireframe.is_null()) {
        LOG(WARNING) << "detected wireframe option in draw desc but was not of boolean type: " << wireframe.dump();
        return false;
      }

      auto polygon_mode = desc_json["polygon_mode"];
      if (polygon_mode.is_object()) {
        auto front = polygon_mode["front"];

        if (front.is_boolean()) {
          desc.polygon_mode.front = front;
        } else if (!front.is_null()) {
          LOG(WARNING) << "detected polygon mode front option in draw desc but was not of boolean type: " << front.dump();
          return false;
        }

        auto back = polygon_mode["back"];

        if (back.is_boolean()) {
          desc.polygon_mode.back = back;
        } else if (!back.is_null()) {
          LOG(WARNING) << "detected polygon mode back option in draw desc but was not of boolean type: " << back.dump();
          return false;
        }
      } else if (!polygon_mode.is_null()) {
        LOG(WARNING) << "detected polygon mode option in draw desc but was not of object type";
        return false;
      }

      return true;
    }
  }  // namespace Resources
}  // namespace Exp
