#pragma once

#include "util.hpp"

namespace ExpGame
{
  class Ui
  {
   public:
    virtual ~Ui() = default;

    virtual void render() = 0;

   private:
  };

  class UiManager
  {
    UiManager();

   public:
    UiManager(const UiManager&) = delete;
    UiManager(UiManager&&)      = delete;
    auto operator=(const UiManager&) -> UiManager& = delete;
    auto operator=(UiManager&&) -> UiManager& = delete;

    ~UiManager() = default;

    static auto instance() -> UiManager&;

    void load_all();

    void render();

    void shutdown();

   private:
    ImGuiContext* context;
    std::vector<std::shared_ptr<Ui>> elements;
  };

  class DebugUi: public Ui
  {
   public:
    virtual ~DebugUi() override = default;

    void render() final;

   private:
    bool show_demo_window = false;
  };
}  // namespace ExpGame
