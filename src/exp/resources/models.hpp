#pragma once

#include "iresource.hpp"
#include "model_meta.hpp"

namespace Exp
{
  namespace R
  {
    class Models: public IResource
    {
      using ModelMap = std::map<std::string, std::shared_ptr<ModelMeta>>;

     public:
      Models()              = default;
      Models(const Models&) = delete;
      Models(Models&&)      = default;
      auto operator=(const Models&) -> Models& = delete;
      auto operator=(Models&&) -> Models& = default;

      void load_all(World& world) final;
      void release() final;

      auto find(std::string id) const noexcept -> ModelMap::const_iterator;
      auto begin() const noexcept -> ModelMap::const_iterator;
      auto end() const noexcept -> ModelMap::const_iterator;

     private:
      ModelMap models;
    };
  }  // namespace R
}  // namespace Exp