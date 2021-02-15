#pragma once

#include "script_meta.hpp"

namespace Exp
{
  namespace R
  {
    class Scripts
    {
      Scripts() = default;

      using ScriptMap = std::map<std::string, ScriptMeta>;

     public:
      Scripts(const Scripts&) = delete;
      Scripts(Scripts&&)      = delete;
      auto operator=(const Scripts&) -> Scripts& = delete;
      auto operator=(Scripts&&) -> Scripts& = delete;

      static auto instance() noexcept -> Scripts&;

      void load_all();

      void release();

      auto find(std::string id) const noexcept -> ScriptMap::const_iterator;
      auto begin() const noexcept -> ScriptMap::const_iterator;
      auto end() const noexcept -> ScriptMap::const_iterator;

      void make_script(std::string id, std::optional<sol::state>& state, std::function<bool(sol::state&)> callback);

     private:
      ScriptMap scripts;
    };
  }  // namespace R
}  // namespace Exp