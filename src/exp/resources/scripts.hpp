#pragma once

#include "script_meta.hpp"

namespace Exp
{
  namespace Resources
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

     private:
      ScriptMap scripts;
    };
  }  // namespace Resources
}  // namespace Exp