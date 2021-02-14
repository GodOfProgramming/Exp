#pragma once

namespace Exp
{
  namespace Settings
  {
    template <typename T>
    class Setting
    {
      using ChangeCallback = std::function<void(Setting&, T)>;

     public:
      using type = T;

      Setting() = default;
      Setting(std::string n, T v)
       : name(n)
       , value(v)
      {}

      auto display_name() const noexcept -> T
      {
        return this->name;
      }

      auto raw() const noexcept -> T
      {
        return this->value;
      }

      operator T()
      {
        return this->value;
      }

      /**
       * @brief First calls all callbacks with the current value of the setting and the to soon be.
       */
      void update(T& v)
      {
        for (const auto& callback : this->callbacks) { callback(*this, v); }
        this->value = v;
      }

      auto operator=(T& v) -> Setting&
      {
        this->value = v;
        return *this;
      }

      auto on_change(ChangeCallback f)
      {
        this->callbacks.push_back(f);
      }

     private:
      std::string name;
      T value;
      std::vector<ChangeCallback> callbacks;
    };
  }  // namespace Settings
}  // namespace Exp