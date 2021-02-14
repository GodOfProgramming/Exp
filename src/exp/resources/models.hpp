#pragma once

#include "model_meta.hpp"

namespace Exp
{
  namespace Resources
  {
    class Models
    {
      Models() = default;

      using ModelMap = std::map<std::string, std::shared_ptr<ModelMeta>>;

     public:
      Models(const Models&) = delete;
      Models(Models&&)      = delete;
      auto operator=(const Models&) -> Models& = delete;
      auto operator=(Models&&) -> Models& = delete;

      static auto instance() noexcept -> Models&;

      void load_all();

      void release();

      auto find(std::string id) const noexcept -> ModelMap::const_iterator;
      auto begin() const noexcept -> ModelMap::const_iterator;
      auto end() const noexcept -> ModelMap::const_iterator;

     private:
      ModelMap models;
    };
  }  // namespace Resources
}  // namespace Exp