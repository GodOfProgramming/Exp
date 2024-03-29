#pragma once

#include "exp/render/draw_description.hpp"
#include "iresource.hpp"
#include "object_meta.hpp"

namespace Exp
{
  namespace R
  {
    class GameObjects: public IResource
    {
      using ObjectMap = std::map<std::string, ObjectMeta>;
      using json      = nlohmann::json;

     public:
      GameObjects()                   = default;
      GameObjects(const GameObjects&) = delete;
      GameObjects(GameObjects&&)      = default;
      auto operator=(const GameObjects&) -> GameObjects& = delete;
      auto operator=(GameObjects&&) -> GameObjects& = default;

      static void add_usertype(sol::state_view state);

      void load_all(World& world) final;

      void release() final;

      auto find(std::string id) const noexcept -> ObjectMap::const_iterator;
      auto begin() const noexcept -> ObjectMap::const_iterator;
      auto end() const noexcept -> ObjectMap::const_iterator;

      auto keys() const noexcept -> std::vector<ObjectMap::key_type>;

      auto get(std::string id) const noexcept -> std::unique_ptr<ObjectMap::mapped_type>;

     private:
      ObjectMap objects;

      auto parse_drawdesc(const json&, Render::DrawDescription& desc) -> bool;
    };
  }  // namespace R
}  // namespace Exp