#pragma once

#include "iresource.hpp"
#include "model_meta.hpp"

namespace Exp
{
  namespace R
  {
    class Models: public IResource
    {
      Models() = default;

      using ModelMap = std::map<std::string, std::shared_ptr<ModelMeta>>;

     public:
      Models(const Models&) = delete;
      Models(Models&&)      = delete;
      auto operator=(const Models&) -> Models& = delete;
      auto operator=(Models&&) -> Models& = delete;

      static auto instance() noexcept -> Models&;

      void load_all() final;
      void release() final;

      auto find(std::string id) const noexcept -> ModelMap::const_iterator;
      auto begin() const noexcept -> ModelMap::const_iterator;
      auto end() const noexcept -> ModelMap::const_iterator;

     private:
      ModelMap models;
    };
  }  // namespace R
}  // namespace Exp