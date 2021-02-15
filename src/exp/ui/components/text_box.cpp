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
       , display_text(t)
      {
        this->enable(true);
      }

      void TextBox::render()
      {
        if (this->script.has_value()) {
          auto& lua = this->script.value();
          auto fn   = lua[this->function];
          if (fn.get_type() == sol::type::function) {
            fn.call(this);
          }
        }

        ImGui::Text("%s", this->display_text.c_str());
      }

      void TextBox::add_usertype(sol::state& state)
      {
        state.new_usertype<TextBox>(
         "TextBox", "display_text", &TextBox::display_text, "text", &TextBox::text, "is_enabled", &TextBox::is_enabled, "enable", &TextBox::enable);
      }

      auto TextBox::text() noexcept -> std::string
      {
        return this->display_text;
      }
    }  // namespace Components
  }    // namespace Ui
}  // namespace Exp
