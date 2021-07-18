#pragma once

#include "iresource.hpp"

namespace Exp
{
  namespace R
  {
    class Scripts: public IResource
    {
      using StateMap  = std::map<std::string, sol::state>;
      using SourceMap = std::map<std::string, std::string>;

     public:
      Scripts()               = default;
      Scripts(const Scripts&) = delete;
      Scripts(Scripts&&)      = default;
      auto operator=(const Scripts&) -> Scripts& = delete;
      auto operator=(Scripts&&) -> Scripts& = default;

      void load_all(World& world) final;
      void release() final;

      auto initialize_state(std::string id) -> bool;
      auto create_env(std::string id, sol::environment& e) -> bool;

     private:
      StateMap states;
      SourceMap sources;
    };
  }  // namespace R
}  // namespace Exp