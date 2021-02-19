#pragma once

#include "exp/resources/id.hpp"
#include "frame.ipp"

namespace Exp
{
  namespace Ui
  {
    namespace Components
    {
      class Frame: public IFrame
      {
       public:
        Frame(const Container& container, std::optional<sol::state>& parent_script);
        ~Frame() final = default;

        static auto from_node(tinyxml2::XMLNode* self, const Container& container, std::optional<sol::state>& parent_script) -> std::shared_ptr<UiComponent>;

        static void add_usertype(sol::state& state);

        auto width() const noexcept -> int final;

        auto height() const noexcept -> int final;

        void render() final;

        auto display_text() noexcept -> std::string final;

       private:
        std::vector<std::shared_ptr<UiComponent>> elements;
        const Container& container;
        std::optional<sol::state>& parent_script;
        std::optional<sol::state> script;
        R::ID<ImGuiID> id;
        glm::ivec2 dim = {};
      };
    }  // namespace Components
  }    // namespace Ui
}  // namespace Exp