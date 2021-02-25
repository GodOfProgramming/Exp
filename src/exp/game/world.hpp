#pragma once

#include "exp/render/renderer.hpp"
#include "object.hpp"

namespace Exp
{
  namespace Game
  {
    class World
    {
      World() = default;

      using ObjectList = std::vector<std::shared_ptr<Object>>;
      using ObjectMap  = std::map<std::size_t, std::shared_ptr<Object>>;

     public:
      World(const World&) = delete;
      World(World&&)      = delete;
      auto operator=(const World&) -> World& = delete;
      auto operator=(World&&) -> World& = delete;

      static auto instance() noexcept -> World&;

      static void add_usertype(sol::state_view state);

      void update();

      void render(Render::Renderer& renderer);

      auto spawn(std::string id, glm::vec3 pos) -> bool;

      void finalize_spawns();

      void release();

     private:
      ObjectList pending_objs;
      ObjectList objects;
      ObjectMap obj_map;
    };
  }  // namespace Game
}  // namespace Exp