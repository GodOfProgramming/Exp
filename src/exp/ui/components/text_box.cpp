#include "text_box.hpp"

namespace Exp
{
  namespace Ui
  {
    namespace Components
    {
      TextBox::TextBox(std::optional<sol::state>& s, std::string f, std::string t)
       : script(s)
       , function(f)
       , text(t)
      {}

      void TextBox::render()
      {
        if (this->script.has_value()) {
          auto& lua = this->script.value();
          auto fn   = lua[this->function];
          if (fn.get_type() == sol::type::function) {
            fn.call(this);
          }
        }

        ImGui::Text("%s", this->text.c_str());
      }

      void TextBox::add_usertype(sol::state& state)
      {
        state.new_usertype<TextBox>("TextBox", "text", &TextBox::text);
      }
    }  // namespace Components
  }    // namespace Ui
}  // namespace Exp
