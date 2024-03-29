#pragma once

#include "ui_component.hpp"

namespace Exp
{
  namespace Ui
  {
    namespace Components
    {
      class Sameline: public UiComponent
      {
       public:
        Sameline(std::optional<sol::environment> script);
        ~Sameline();

        static auto from_node(tinyxml2::XMLNode* self, std::optional<sol::environment> env) -> std::shared_ptr<UiComponent>;

        void render() final;

        auto display_text() noexcept -> std::string final;
      };
    }  // namespace Components
  }    // namespace Ui
}  // namespace Exp