#pragma once

#include "exp/resources/id.hpp"
#include "text_box.ipp"

namespace Exp
{
  namespace Ui
  {
    namespace Components
    {
      class TextBox: public ITextBox
      {
       public:
        TextBox(std::optional<sol::state>& script, std::string fn, std::string text);
        ~TextBox() final = default;

        static auto from_node(tinyxml2::XMLNode* self, std::optional<sol::state>& script) -> std::shared_ptr<UiComponent>;

        static void add_usertype(sol::state& state);

        void render() final;

        auto display_text() noexcept -> std::string final;

       private:
        std::optional<sol::state>& script;

        std::string function;
        std::string text;
        std::string btn_text;
        R::ID<int> btn_id;
        std::optional<std::string> button_fn;
      };
    }  // namespace Components
  }    // namespace Ui
}  // namespace Exp