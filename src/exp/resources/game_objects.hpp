#pragma once

#include "exp/gl/draw_description.hpp"
#include "iresource.hpp"
#include "object_meta.hpp"

namespace Exp
{
  namespace Resources
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

      void load_all();

      void release() final;

      auto find(std::string id) const noexcept -> ObjectMap::const_iterator;
      auto begin() const noexcept -> ObjectMap::const_iterator;
      auto end() const noexcept -> ObjectMap::const_iterator;

     private:
      ObjectMap objects;

      auto parse_drawdesc(const json&, GL::DrawDescription& desc) -> bool;
    };
  }  // namespace Resources
}  // namespace Exp