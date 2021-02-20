#pragma once

#include "exp/resources/id.hpp"
#include "exp/ui/components/container.hpp"

namespace Exp
{
  namespace Ui
  {
    namespace Components
    {
      class Frame: public Container
      {
       public:
        Frame(std::optional<sol::state_view> script, const Container& container);
        ~Frame() final;

        static auto from_node(tinyxml2::XMLNode* self, std::optional<sol::state_view> script, const Container& container) -> std::shared_ptr<UiComponent>;

        static void add_usertype(sol::state_view& state);

        auto width() const noexcept -> int final;

        auto height() const noexcept -> int final;

        void render() final;

        auto display_text() noexcept -> std::string final;

       private:
        const Container& container;
        R::ID<ImGuiID> imgui_id;
        glm::ivec2 dim = {};
        glm::ivec2 pos = {};
      };
    }  // namespace Components
  }    // namespace Ui
}  // namespace Exp