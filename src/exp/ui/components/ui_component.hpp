#pragma once

#include "exp/resources/id.hpp"

namespace Exp
{
  namespace Ui
  {
    class UiComponent
    {
     public:
      virtual ~UiComponent();

      virtual void render() = 0;

      static auto from_node(tinyxml2::XMLElement* self, std::shared_ptr<UiComponent>) -> bool;

      virtual auto display_text() noexcept -> std::string = 0;

      void enable(bool) noexcept;

      auto is_enabled() const noexcept -> bool;

      auto add_element(std::shared_ptr<UiComponent> el) -> bool;

      auto remove_element(const UiComponent& el);

      std::string id;

     protected:
      UiComponent(std::optional<sol::state_view> script);
      void release();

      static auto has_attr_button(tinyxml2::XMLElement* self, std::string& fn) -> bool;
      static auto has_attr_collapsed(tinyxml2::XMLElement* self, bool& is_collapsed) -> bool;
      static auto has_attr_fixed(tinyxml2::XMLElement* self, bool& is_fixed) -> bool;
      static auto has_attr_fn(tinyxml2::XMLElement* self, std::string& fn) -> bool;
      static auto has_attr_height(tinyxml2::XMLElement* self, float& height) -> bool;
      static auto has_attr_script(tinyxml2::XMLElement* self, std::string& script) -> bool;
      static auto has_attr_title(tinyxml2::XMLElement* self, std::string& title) -> bool;
      static auto has_attr_while(tinyxml2::XMLElement* self, std::string& fn) -> bool;
      static auto has_attr_width(tinyxml2::XMLElement* self, float& width) -> bool;
      static auto has_attr_x(tinyxml2::XMLElement* self, float& x) -> bool;
      static auto has_attr_y(tinyxml2::XMLElement* self, float& y) -> bool;

      auto eval_if(sol::state_view scriptopt) const -> bool;

      void render_children();

      std::optional<sol::state_view> script;

      std::optional<std::string> if_fn;

      sol::table userdata;

     private:
      static auto has_attr_id(tinyxml2::XMLElement* self, std::string& id) -> bool;
      static auto has_attr_if(tinyxml2::XMLElement* self, std::string& fn) -> bool;

      std::vector<std::shared_ptr<UiComponent>> elements;
      std::map<std::string, std::shared_ptr<UiComponent>> element_map;
      bool enabled = false;
      R::ID<std::size_t> secret_id;
    };
  }  // namespace Ui
}  // namespace Exp