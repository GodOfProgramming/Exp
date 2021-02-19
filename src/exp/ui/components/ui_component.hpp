#pragma once

namespace Exp
{
  namespace Ui
  {
    class UiComponent
    {
     public:
      virtual ~UiComponent() = default;

      virtual void render() = 0;

      virtual auto display_text() noexcept -> std::string = 0;

      void enable(bool) noexcept;

      auto is_enabled() const noexcept -> bool;

     protected:
      static auto has_attr_button(tinyxml2::XMLElement* self, std::string& fn) -> bool;
      static auto has_attr_collapsed(tinyxml2::XMLElement* self, bool& is_collapsed) -> bool;
      static auto has_attr_fixed(tinyxml2::XMLElement* self, bool& is_fixed) -> bool;
      static auto has_attr_fn(tinyxml2::XMLElement* self, std::string& fn) -> bool;
      static auto has_attr_height(tinyxml2::XMLElement* self, float& height) -> bool;
      static auto has_attr_if(tinyxml2::XMLElement* self, std::string& fn) -> bool;
      static auto has_attr_script(tinyxml2::XMLElement* self, std::string& script) -> bool;
      static auto has_attr_title(tinyxml2::XMLElement* self, std::string& title) -> bool;
      static auto has_attr_while(tinyxml2::XMLElement* self, std::string& fn) -> bool;
      static auto has_attr_width(tinyxml2::XMLElement* self, float& width) -> bool;
      static auto has_attr_x(tinyxml2::XMLElement* self, float& x) -> bool;
      static auto has_attr_y(tinyxml2::XMLElement* self, float& y) -> bool;

      auto eval_if(std::optional<sol::state>& scriptopt) const -> bool;

      std::optional<std::string> if_fn;

      sol::table userdata;

     private:
      bool enabled = false;
    };
  }  // namespace Ui
}  // namespace Exp