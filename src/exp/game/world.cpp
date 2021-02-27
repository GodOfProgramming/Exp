#include "world.hpp"

#include "exp/constants.hpp"
#include "exp/render/app_window.hpp"
#include "exp/resources/game_objects.hpp"

namespace Exp
{
  namespace Game
  {
    auto World::instance() noexcept -> World&
    {
      static World world;
      return world;
    }

    void World::add_usertype(sol::state_view state)
    {
      if (state[Lua::Usertypes::Game::WORLD].get_type() == sol::type::none) {
        state.new_usertype<World>(
         Lua::Usertypes::Game::WORLD, "instance", &World::instance, "spawn", &World::spawn, "lookup", &World::lookup, "count", &World::count);
      }
    }

    void World::update()
    {
      for (auto obj : this->objects) { obj->update(); }
    }

    void World::update(Util::ThreadPool& tp)
    {
      std::mutex m;
      std::condition_variable cv;
      std::atomic<std::size_t> completed   = 0;
      const std::size_t updates_to_be_made = this->objects.size();

      for (auto obj : this->objects) {
        tp.enqueue(Util::ThreadPriority::TOP, [obj, updates_to_be_made, &completed, &cv] {
          obj->update();
          completed++;
          if (completed == updates_to_be_made) {
            cv.notify_one();
          }
        });
      }

      std::unique_lock<std::mutex> lk(m);
      cv.wait(lk);
    }

    void World::render(Render::Renderer& renderer)
    {
      renderer.render_to(Render::AppWindow::instance(), this->objects);
    }

    auto World::spawn(std::string id, glm::vec3 pos) -> bool
    {
      auto& objects = R::GameObjects::instance();
      auto iter     = objects.find(id);
      if (iter == objects.end()) {
        LOG(WARNING) << "unable to spawn object " << id;
        return false;
      }
      this->pending_objs.push_back(std::make_shared<Object>(iter->second, pos));
      return true;
    }

    void World::finalize_spawns()
    {
      if (this->pending_objs.size() > 0) {
        if (this->objects.capacity() < this->objects.size() + this->pending_objs.size()) {
          this->objects.reserve(this->pending_objs.size());
        }
        for (auto obj : this->pending_objs) {
          this->objects.push_back(obj);
          this->obj_map[obj->id()] = obj;
        }
        this->pending_objs.clear();
      }
    }

    auto World::lookup(std::size_t id) -> std::shared_ptr<Object>
    {
      auto iter = this->obj_map.find(id);
      if (iter == this->obj_map.end()) {
        return nullptr;
      }
      return iter->second;
    }

    auto World::count() -> std::size_t
    {
      return this->pending_objs.size() + this->objects.size();
    }

    void World::release()
    {
      this->pending_objs.clear();
      this->obj_map.clear();
      this->objects.clear();
    }
  }  // namespace Game
}  // namespace Exp
