#pragma once

#include "exp/ui/ui_component.hpp"

namespace Exp
{
  namespace Ui
  {
    namespace Components
    {
      class DebugUi: public UiComponent
      {
       public:
        virtual ~DebugUi() final = default;

        void render() final;

       private:
        bool show_demo_window  = false;
        bool show_shader_debug = false;
        bool show_gl_errors    = false;
      };
    }  // namespace Components
  }    // namespace Ui
}  // namespace Exp