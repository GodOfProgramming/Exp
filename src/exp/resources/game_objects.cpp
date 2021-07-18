#include "game_objects.hpp"

#include "animations.hpp"
#include "exp/constants.hpp"
#include "exp/io.hpp"
#include "models.hpp"
#include "shaders.hpp"

namespace Exp
{
  namespace R
  {
    void GameObjects::add_usertype(sol::state_view state)
    {
      if (state[Lua::Usertypes::R::GAME_OBJECTS].get_type() == sol::type::none) {
        ObjectMeta::add_usertype(state);
        state.new_usertype<GameObjects>(Lua::Usertypes::R::GAME_OBJECTS, "keys", &GameObjects::keys, "get", &GameObjects::get);
      }
    }

    void GameObjects::load_all(World& world)
    {
      LOG(INFO) << "loading game objects";
      auto* shaders    = world.get_resource<Shaders>();
      auto* models     = world.get_resource<Models>();
      auto* animations = world.get_resource<Animations>();

      IO::iterate_dir_with_namespace(Cfg::Dir::GAME_OBJECTS, std::string{ "exp" }, [&](const std::filesystem::path path, const std::string& nspace) {
        IResource::load_json_file(path, [&](const json& objects) {
          if (!objects.is_object()) {
            LOG(WARNING) << "shader json is not in proper format, first type is not object";
            return;
          }

          for (const auto item : objects.items()) {
            auto id  = nspace + "." + item.key();
            auto obj = item.value();

            LOG(INFO) << "loading game object " << id;

            std::string construct_fn;
            if (!ObjectMeta::has_construct_fn(obj, construct_fn)) {
              continue;
            }

            std::string update_fn;
            if (!ObjectMeta::has_update_fn(obj, update_fn)) {
              continue;
            }

            std::string shader_id;
            if (!ObjectMeta::has_shader_id(obj, shader_id)) {
              continue;
            }

            std::string model_id;
            if (!ObjectMeta::has_model_id(obj, model_id)) {
              continue;
            }

            std::string animation_id;
            ObjectMeta::has_animation_id(obj, animation_id);

            std::string script_id;
            if (!ObjectMeta::has_script_id(obj, script_id)) {
              continue;
            }

            /* Shader */

            auto shader_iter = shaders->find_program(shader_id);
            if (shader_iter == shaders->program_end()) {
              LOG(WARNING) << "cannot find shader with id " << shader_id;
              continue;
            }

            auto shader = shader_iter->second;

            if (!shader->is_valid()) {
              LOG(WARNING) << "cannot load item, shader " << shader_id << " not valid";
              continue;
            }

            /* Model */

            auto model_iter = models->find(model_id);
            if (model_iter == models->end()) {
              LOG(WARNING) << "cannot find model with id " << model_id;
              continue;
            }

            auto model = model_iter->second;

            /* Animation */

            std::shared_ptr<R::AnimationMeta> animation;

            auto animation_iter = animations->find(animation_id);
            if (animation_iter != animations->end()) {
              animation = animation_iter->second;
            }

            /* Draw Description */

            Render::DrawDescription desc;

            auto drawdesc_json = obj[JSON::Keys::GAME_OBJECT_DRAW_DESC];
            if (drawdesc_json.is_object()) {
              if (!this->parse_drawdesc(drawdesc_json, desc)) {
                LOG(WARNING) << "unable to parse partial or full draw desc on object, using what was valid";
              }
            } else if (!drawdesc_json.is_null()) {
              LOG(WARNING) << "detected draw description in config but desc was not of type object";
              continue;
            }

            /* Finalize */

            ObjectMeta meta;
            meta.id           = id;
            meta.construct_fn = construct_fn;
            meta.update_fn    = update_fn;
            meta.shader_id    = shader_id;
            meta.model_id     = model_id;
            meta.animation_id = animation_id;
            meta.script_id    = script_id;

            meta.shader    = shader;
            meta.model     = model;
            meta.animation = animation;

            meta.drawdesc = desc;

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
