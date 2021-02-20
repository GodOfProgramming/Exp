#include "container.hpp"

namespace Exp
{
  namespace Ui
  {
    namespace Components
    {
      Container::Container(std::optional<sol::state_view> script)
       : UiComponent(script)
      {}

      Container::~Container() {}

      auto Container::from_node(tinyxml2::XMLElement* self, std::shared_ptr<Container> cmp, glm::ivec2 parent_dim) -> bool
      {
        if (!UiComponent::from_node(self, cmp)) {
          return false;
        }

        float width_percent;
        if (UiComponent::has_attr_width(self, width_percent)) {
          cmp->dim.x = static_cast<int>(parent_dim.x * width_percent / 100.0);
        } else {
          cmp->dim.x = static_cast<int>(parent_dim.x / 2);
        }

        float height_percent;
        if (UiComponent::has_attr_height(self, height_percent)) {
          cmp->dim.y = static_cast<int>(parent_dim.y * height_percent / 100.0);
        } else {
          cmp->dim.y = static_cast<int>(parent_dim.y / 2);
        }

        float x_percent;
        if (UiComponent::has_attr_x(self, x_percent)) {
          cmp->pos.x = parent_dim.x * x_percent / 100.0;
        } else {
          cmp->pos.x = 0;
        }

        float y_percent;
        if (UiComponent::has_attr_y(self, y_percent)) {
          cmp->pos.y = parent_dim.y * y_percent / 100.0;
        } else {
          cmp->pos.y = 0;
        }
        return true;
      }

      void Container::release()
      {
        UiComponent::release();
      }
    }  // namespace Components
  }    // namespace Ui
}  // namespace Exp