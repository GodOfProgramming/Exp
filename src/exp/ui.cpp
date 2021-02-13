#include "ui.hpp"

#include "constants.hpp"
#include "game.hpp"
#include "gl.hpp"
#include "io.hpp"
#include "resources.hpp"
#include "window.hpp"

namespace ExpGame
{
  namespace Ui
  {
    using IO::File;
    using Window::AppWindow;

    auto UiManager::instance() -> UiManager&
    {
      static UiManager manager;
      return manager;
    }

    UiManager::UiManager()
    {
      IMGUI_CHECKVERSION();
      this->context = ImGui::CreateContext();

      auto& window = AppWindow::instance();
      ImGui_ImplGlfw_InitForOpenGL(*window, false);
      ImGui_ImplOpenGL3_Init();
      ImGui::StyleColorsDark();
      ImGui::GetIO().MouseDrawCursor = false;
      ImGui::GetIO().IniFilename     = nullptr;
    }

    void UiManager::load_all()
    {
      std::filesystem::directory_iterator iter(UI_DIRECTORY);

      for (auto dir : iter) {
        LOG(INFO) << "loading ui " << dir;
        auto res = File::load(dir);

        if (!res) {
          LOG(WARNING) << "unable to load Ui Xml " << dir;
          continue;
        }

        auto file = res.ok_val();

        this->parse(std::move(file.data));
      }

      this->elements.push_back(std::make_unique<DebugUi>());
    }

    auto UiManager::parse(std::string&& xml) -> bool
    {
      tinyxml2::XMLDocument document;
      auto res = document.Parse(xml.c_str(), xml.size());
      if (res != tinyxml2::XML_SUCCESS) {
        LOG(WARNING) << "unable to parse Ui Xml: " << res;
      }

      auto first               = document.FirstChild();
      std::string element_type = first->Value();

      if (element_type == UI_EL_WINDOW) {
        auto window = std::make_unique<WindowUi>();
        if (window->parse(first)) {
          this->elements.push_back(std::move(window));
          return true;
        } else {
          return false;
        }
      } else {
        return false;
      }
    }

    void UiManager::render()
    {
      ImGui_ImplOpenGL3_NewFrame();
      ImGui_ImplGlfw_NewFrame();
      ImGui::NewFrame();

      for (auto& element : this->elements) { element->render(); }

      ImGui::Render();
      ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    }

    void UiManager::shutdown()
    {
      ImGui_ImplOpenGL3_Shutdown();
      ImGui_ImplGlfw_Shutdown();
      ImGui::DestroyContext(this->context);
    }

    WindowUi::WindowUi(std::string t)
     : title(t)
     , dim({ 0, 0 })
     , pos({ 0, 0 })
     , hide(false)
     , initial_render(false)
     , is_collapsed(false)
    {}

    void WindowUi::render()
    {
      if (this->hide) {
        return;
      }

      ImGuiWindowFlags flags = 0;
      flags |= ImGuiWindowFlags_NoResize;

      ImVec2 size{ static_cast<float>(this->dim.x), static_cast<float>(this->dim.y) };
      ImGui::SetNextWindowSize(size);

      if (!this->initial_render) {
        ImVec2 pos{ static_cast<float>(this->pos.x), static_cast<float>(this->pos.y) };
        ImGui::SetNextWindowPos(pos);
        ImGui::SetNextWindowCollapsed(this->is_collapsed);
        this->initial_render = true;
      }

      bool is_open = true;
      ImGui::Begin(this->title.c_str(), &is_open, flags);

      for (const auto& element : elements) { element->render(); }

      ImGui::End();

      if (!is_open && this->lua.has_value()) {
        this->hide   = true;
        auto& script = this->lua.value();
        auto fn      = script["on_close"];
        if (fn.get_type() == sol::type::function) {
          fn.call(this);
        }
      }
    }

    auto WindowUi::parse(tinyxml2::XMLNode* self) -> bool
    {
      auto element = self->ToElement();
      if (element == nullptr) {
        LOG(WARNING) << "unable to convert window to element type";
        return false;
      }

      auto title_attr = element->FindAttribute("title");

      if (title_attr == nullptr) {
        LOG(WARNING) << "unable to get title value of window";
        return false;
      }

      auto collapsed_attr = element->FindAttribute("collapsed");

      if (collapsed_attr != nullptr) {
        bool collapsed = false;
        auto res       = collapsed_attr->QueryBoolValue(&collapsed);
        if (res != tinyxml2::XML_SUCCESS) {
          LOG(WARNING) << "unable to parse width: " << res;
          return false;
        }
        this->is_collapsed = collapsed;
      }

      this->title = title_attr->Value();

      auto& window = AppWindow::instance();
      auto size    = window.get_size();

      {
        auto width_attr = element->FindAttribute("width");
        if (width_attr == nullptr) {
          this->dim.x = size.x / 2;
        } else {
          float percent{ 0.0 };
          auto res = width_attr->QueryFloatValue(&percent);
          if (res != tinyxml2::XML_SUCCESS) {
            LOG(WARNING) << "unable to parse width: " << res;
            return false;
          }
          this->dim.x = size.x * percent / 100.0;
        }
      }

      {
        auto height_attr = element->FindAttribute("height");
        if (height_attr == nullptr) {
          this->dim.y = size.y / 2;
        } else {
          float percent{ 0.0 };
          auto res = height_attr->QueryFloatValue(&percent);
          if (res != tinyxml2::XML_SUCCESS) {
            LOG(WARNING) << "unable to parse height: " << res;
            return false;
          }
          this->dim.y = size.y * percent / 100.0;
        }
      }

      {
        auto x_attr = element->FindAttribute("x");
        if (x_attr == nullptr) {
          this->pos.x = 0;
        } else {
          float percent{ 0.0 };
          auto res = x_attr->QueryFloatValue(&percent);
          if (res != tinyxml2::XML_SUCCESS) {
            LOG(WARNING) << "unable to parse x pos: " << res;
            return false;
          }
          this->pos.x = size.x * percent / 100.0;
        }
      }

      {
        auto y_attr = element->FindAttribute("y");
        if (y_attr == nullptr) {
          this->pos.y = 0;
        } else {
          float percent{ 0.0 };
          auto res = y_attr->QueryFloatValue(&percent);
          if (res != tinyxml2::XML_SUCCESS) {
            LOG(WARNING) << "unable to parse y pos: " << res;
            return false;
          }
          this->pos.y = size.y * percent / 100.0;
        }
      }

      {
        auto script_attr = element->FindAttribute("script");
        if (script_attr != nullptr) {
          std::string script_key = script_attr->Value();
          auto& scripts          = Resources::Scripts::instance();
          auto script_iter       = scripts.find(script_key);
          if (script_iter == scripts.end()) {
            LOG(WARNING) << "unable to load script " << script_key;
            return false;
          }

          auto& script = script_iter->second;

          sol::state state;
          auto res = state.load(script.src);
          if (!res.valid()) {
            LOG(WARNING) << "script " << script_key << " is invalid";
            return false;
          }

          auto exec_res = res();

          if (!exec_res.valid()) {
            LOG(WARNING) << "script " << script_key << " was not able to run";
            return false;
          }

          state.open_libraries(sol::lib::base, sol::lib::string);

          state.new_usertype<glm::ivec2>("ivec2", "x", &glm::ivec2::x, "y", &glm::ivec2::y);
          state.new_usertype<WindowUi>("WindowUi", "title", &WindowUi::title, "dim", &WindowUi::dim, "pos", &WindowUi::pos);
          state.new_usertype<Game::Info>("GameInfo", "fps", &Game::Info::fps, "frames", &Game::Info::frames);

          state.set("game_info", &Game::Info::instance());

          this->lua = std::move(state);
        }
      }

      return true;
    }

    void DebugUi::render()
    {
      if (ImGui::BeginMainMenuBar()) {
        if (ImGui::BeginMenu("Debug")) {
          if (ImGui::MenuItem("Demo Window")) {
            this->show_demo_window = !this->show_demo_window;
          }

          if (ImGui::MenuItem("Shaders")) {
            this->show_shader_debug = !this->show_shader_debug;
          }

          if (ImGui::MenuItem("OpenGL Errors")) {
            this->show_gl_errors = !this->show_gl_errors;
          }

          if (ImGui::MenuItem("Exit")) {
            auto& window = AppWindow::instance();
            window.close();
          }
          ImGui::EndMenu();
        }
        ImGui::EndMainMenuBar();
      }

      if (this->show_demo_window) {
        ImGui::ShowDemoWindow();
      }

      if (this->show_shader_debug) {
        static ShaderUi ui;
        ui.render();
      }

      if (this->show_gl_errors) {
        static GlErrorsUi ui;
        ui.render();
      }
    }

    void ShaderUi::render()
    {
      auto& shaders = Resources::Shaders::instance();

      if (!this->initial_render) {
        auto& window = AppWindow::instance();
        auto size    = window.get_size();
        ImGui::SetNextWindowSize(ImVec2{ static_cast<float>(size.x) * 0.8f, static_cast<float>(size.y) * 0.8f });
        ImGui::SetNextWindowPos(ImVec2{ static_cast<float>(size.x) * 0.1f, static_cast<float>(size.y) * 0.1f });
        this->initial_render = true;
      }

      ImGui::Begin("Shaders");

      std::size_t id = 0;
      for (auto cfg_iter = shaders.cache_begin(); cfg_iter != shaders.cache_end(); cfg_iter++, id++) {
        auto& program_id  = cfg_iter->first;
        auto& shader_meta = cfg_iter->second;

        ImGui::Text("ID: \"%s\"", program_id.c_str());

        const auto program_iter = shaders.find_program(cfg_iter->first);

        bool loaded = program_iter != shaders.program_end();

        const char* status_txt = loaded ? "Yes" : "No";
        ImGui::Text("Loaded: %s", status_txt);

        if (loaded) {
          const auto& program = program_iter->second;

          auto& window = AppWindow::instance();
          auto dim     = window.get_size();
          auto indent  = dim.x * 0.025f;

          ImGui::Indent(indent);
          ImGui::Text("Vertex: %s", shader_meta.vertex.file.c_str());
          {
            ImGui::Indent(indent);
            auto vertex             = shaders.find_shader(shader_meta.vertex.file)->second;
            auto* vertex_status_str = vertex->is_valid() ? "Ok" : "Error";
            ImGui::Text("Status: %s", vertex_status_str);
            if (!vertex->is_valid()) {
              ImGui::Text("%s", shader_meta.vertex.error.c_str());
            }
            ImGui::Indent(-indent);
          }
          ImGui::Indent(-indent);

          ImGui::Indent(indent);
          ImGui::Text("Fragment: %s", shader_meta.fragment.file.c_str());
          {
            ImGui::Indent(indent);
            auto fragment             = shaders.find_shader(shader_meta.fragment.file)->second;
            auto* fragment_status_str = fragment->is_valid() ? "Ok" : "Error";
            ImGui::Text("Status: %s", fragment_status_str);
            if (!fragment->is_valid()) {
              ImGui::Text("%s", shader_meta.vertex.error.c_str());
            }
            ImGui::Indent(-indent);
          }
          ImGui::Text("%s", shader_meta.fragment.error.c_str());
          ImGui::Indent(-indent);

          if (!program->is_valid()) {
            ImGui::Text("Link Error: %s", shader_meta.link_error.c_str());
          }
        }

        ImGui::PushID(id);
        if (ImGui::Button("Reload")) {
          shaders.reload_program(program_id);
        }
        ImGui::PopID();
        ImGui::Separator();
      }

      ImGui::End();
    }

    void GlErrorsUi::render()
    {
      if (!this->initial_render) {
        auto& window = AppWindow::instance();
        auto size    = window.get_size();
        ImGui::SetNextWindowSize(ImVec2{ static_cast<float>(size.x) * 0.8f, static_cast<float>(size.y) * 0.8f });
        ImGui::SetNextWindowPos(ImVec2{ static_cast<float>(size.x) * 0.1f, static_cast<float>(size.y) * 0.1f });
        this->initial_render = true;
      }

      auto& errors = GL::ErrorMap::instance();

      ImGui::Begin("OpenGL Errors");

      if (errors.error_count() == 0) {
        ImGui::Text("No Errors Detected");
      } else {
        auto& window = AppWindow::instance();
        auto dim     = window.get_size();
        auto indent  = dim.x * 0.025f;

        ImGuiID first = 1;
        for (const auto& kvp : errors) {
          auto id     = kvp.first;
          auto& error = kvp.second;

          ImGui::Text("Code: %u", id);
          ImGui::Text("Desc: %s", error.desc.c_str());
          if (ImGui::BeginChild(first++, { 0, dim.x * 0.2f }, true)) {
            for (const auto& file_lines_pair : error.occurrences) {
              auto& file  = file_lines_pair.first;
              auto& lines = file_lines_pair.second;
              ImGui::Text("File %s", file.c_str());
              for (const auto& line : lines) {
                ImGui::Indent(indent);
                ImGui::Text("%d: %lu", line.first, line.second);
                ImGui::Indent(-indent);
              }
            }
          }
          ImGui::EndChild();
        }
      }

      ImGui::End();
    }
  }  // namespace Ui
}  // namespace ExpGame
