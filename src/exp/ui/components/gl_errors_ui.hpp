#pragma once

#include "exp/ui/ui_component.hpp"

namespace Exp
{
  namespace Ui
  {
    namespace Components
    {
      class GlErrorsUi: public UiComponent
      {
       public:
        virtual ~GlErrorsUi() = default;

        void render() final;

        auto text() noexcept -> std::string final;

       private:
        bool initial_render = false;
      };
    }  // namespace Components
  }    // namespace Ui
}  // namespace Exp