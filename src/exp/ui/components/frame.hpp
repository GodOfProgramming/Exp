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
        Frame(std::optional<sol::state>& parent_script);

        static auto from_node(tinyxml2::XMLNode* self, std::optional<sol::state>& parent_script) -> std::shared_ptr<UiComponent>;

        static void add_usertype(sol::state& state);

        void render() final;

        auto text() noexcept -> std::string final;

       private:
        std::vector<std::shared_ptr<UiComponent>> elements;
        std::optional<sol::state>& parent_script;
        std::optional<sol::state> script;
        R::ID<ImGuiID> id;
      };
    }  // namespace Components
  }    // namespace Ui
}  // namespace Exp