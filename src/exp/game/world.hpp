#pragma once

#include "exp/util/thread_pool.hpp"
#include "object.hpp"

namespace Exp
{
  namespace Game
  {
    class World
    {
      using ObjectList  = std::vector<std::shared_ptr<Object>>;
      using ObjectMap   = std::map<std::size_t, std::shared_ptr<Object>>;
      using ResourceMap = std::unordered_map<std::type_index, std::unique_ptr<std::any>>;

     public:
      World()             = default;
      World(const World&) = delete;
      World(World&&)      = default;
      auto operator=(const World&) -> World& = delete;
      auto operator=(World&&) -> World& = default;

      static void add_usertype(sol::state_view state);

      void update();

      void update(Util::ThreadPool& tp);

      auto spawn(std::string id, glm::vec3 pos) -> bool;

      void finalize_spawns();

      auto lookup(std::size_t id) -> std::shared_ptr<Object>;

      auto count() const noexcept -> std::size_t;

      auto entities() const noexcept -> const ObjectList&;

      void release();

      template <typename T>
      void add_resource(T&& resource);

      template <typename T>
      auto get_resource() -> T*;

     private:
      std::mutex pending_obj_lock;
      ObjectList pending_objs;
      ObjectList objects;
      ObjectMap obj_map;
      ResourceMap resources;
    };

    template <typename T>
    void World::add_resource(T&& resource)
    {
      static_assert(std::is_copy_constructible_v<T>, "T must be copy constructable");
      this->resources[std::type_index(typeid(T))] = std::make_unique<std::any>(std::move(resource));
    }

    template <typename T>
    auto World::get_resource() -> T*
    {
      try {
        return &std::any_cast<T&>(*this->resources[std::type_index(typeid(T))]);
      } catch (const std::bad_any_cast&) {
        return nullptr;
      }
    }
  }  // namespace Game
}  // namespace Exp