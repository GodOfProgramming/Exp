#pragma once

#include "ui_component.hpp"

namespace Exp
{
  namespace Ui
  {
    namespace Components
    {
      class Container: public UiComponent
      {
       public:
        Container(std::optional<sol::state_view> script);
        virtual ~Container();

        static auto from_node(tinyxml2::XMLElement* self, std::shared_ptr<Container> cmp, glm::ivec2 parent_dim) -> bool;

        virtual auto width() const noexcept -> int  = 0;
        virtual auto height() const noexcept -> int = 0;

       protected:
        void release();
        glm::ivec2 dim = {};
        glm::ivec2 pos = {};
      };
    }  // namespace Components
  }    // namespace Ui
}  // namespace Exp