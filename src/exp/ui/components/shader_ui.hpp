#pragma once

#include "ui_component.hpp"

namespace Exp
{
  namespace Ui
  {
    namespace Components
    {
      class ShaderUi: public UiComponent
      {
       public:
        ShaderUi();
        virtual ~ShaderUi() final = default;

        void render() final;

        auto display_text() noexcept -> std::string final;

       private:
        bool initial_render = false;
      };
    }  // namespace Components
  }    // namespace Ui
}  // namespace Exp