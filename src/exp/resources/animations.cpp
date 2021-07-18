#include "animations.hpp"

#include "exp/constants.hpp"
#include "exp/io.hpp"
#include "textures.hpp"

namespace Exp
{
  namespace R
  {
    void Animations::load_all(World& world)
    {
      auto* textures = world.get_resource<Textures>();
      IO::iterate_dir_with_namespace(Cfg::Dir::ANIMATIONS, std::string{ "exp" }, [&](const std::filesystem::path path, const std::string& nspace) {
        IResource::load_json_file(path, [&](const nlohmann::json& objects) {
          if (!objects.is_object()) {
            LOG(WARNING) << "shader json is not in proper format, first type is not object";
            return;
          }

          for (const auto item : objects.items()) {
            auto id  = nspace + "." + item.key();
            auto obj = item.value();

            LOG(INFO) << "loading animations " << id;

            if (!obj.is_object()) {
              LOG(WARNING) << "animation is not object";
              continue;
            }

            std::size_t width;
            if (!AnimationMeta::has_width(obj, width)) {
              LOG(WARNING) << "animation lacks width attribute";
              continue;
            }

            std::size_t height;
            if (!AnimationMeta::has_height(obj, height)) {
              LOG(WARNING) << "animation lacks height attribute";
              continue;
            }

            std::string texture_id;
            if (!AnimationMeta::has_texture_id(obj, texture_id)) {
              LOG(WARNING) << "animation lacks image";
              continue;
            }

            AnimationMeta::ActionMap actions;
            if (!AnimationMeta::parse_actions(obj, actions)) {
              LOG(WARNING) << "could not parse animation actions";
              continue;
            }

            /* Texture */

            auto texture_iter = textures->find(texture_id);
            if (texture_iter == textures->end()) {
              LOG(WARNING) << "cannot find texture with id " << texture_id;
              continue;
            }

            auto texture = texture_iter->second;

            auto meta        = std::make_shared<AnimationMeta>();
            meta->width      = width;
            meta->height     = height;
            meta->action_map = std::move(actions);
            meta->texture    = texture;

            this->animation_map.emplace(id, meta);
          }
        });
      });
    }

    void Animations::release()
    {
      this->animation_map.clear();
    }

    auto Animations::find(std::string id) const noexcept -> AnimationMap::const_iterator
    {
      return this->animation_map.find(id);
    }

    auto Animations::begin() const noexcept -> AnimationMap::const_iterator
    {
      return this->animation_map.begin();
    }

    auto Animations::end() const noexcept -> AnimationMap::const_iterator
    {
      return this->animation_map.end();
    }
  }  // namespace R
}  // namespace Exp
