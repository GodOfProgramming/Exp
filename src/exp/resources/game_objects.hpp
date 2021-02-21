#pragma once

#include "exp/gl/draw_description.hpp"
#include "iresource.hpp"
#include "object_meta.hpp"

namespace Exp
{
  namespace R
  {
    class GameObjects: public IResource
    {
      GameObjects() = default;

      using ObjectMap = std::map<std::string, ObjectMeta>;
      using json      = nlohmann::json;

     public:
      GameObjects(const GameObjects&) = delete;
      GameObjects(GameObjects&&)      = delete;
      auto operator=(const GameObjects&) -> GameObjects& = delete;
      auto operator=(GameObjects&&) -> GameObjects& = delete;

      static auto instance() noexcept -> GameObjects&;

      static void add_usertype(sol::state_view state);

      void load_all() final;

      void release() final;

      auto find(std::string id) const noexcept -> ObjectMap::const_iterator;
      auto begin() const noexcept -> ObjectMap::const_iterator;
      auto end() const noexcept -> ObjectMap::const_iterator;

      auto keys() const noexcept -> std::vector<ObjectMap::key_type>;

      auto get(std::string id) const noexcept -> std::unique_ptr<ObjectMap::mapped_type>;

     private:
      ObjectMap objects;

      auto parse_drawdesc(const json&, GL::DrawDescription& desc) -> bool;
    };
  }  // namespace R
}  // namespace Exp