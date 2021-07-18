#pragma once

#include "exp/game/world.hpp"

namespace Exp
{
  namespace R
  {
    class IResource
    {
     protected:
      using World = Game::World;

     public:
      virtual ~IResource()                = default;
      virtual void load_all(World& world) = 0;
      virtual void release()              = 0;

     protected:
      static void load_src_file(std::filesystem::path path, std::function<void(const std::string_view&)> callback);
      static void load_json_file(std::filesystem::path path, std::function<void(const nlohmann::json&)> callback);
    };
  }  // namespace R
}  // namespace Exp