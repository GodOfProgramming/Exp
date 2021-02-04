#include "resources.hpp"

namespace ExpGame
{
  namespace Resources
  {
    Shaders::Shaders() {}
    auto Shaders::instance() -> Shaders&
    {
      static Shaders shaders;
      return shaders;
    }

    auto Shaders::load_program(std::string id, std::string vertex_id, std::string fragment_id) -> bool
    {
      auto viter = this->shader_map.find(vertex_id);
      auto fiter = this->shader_map.find(fragment_id);

      if (viter == this->shader_map.end() || fiter == this->shader_map.end()) {
        LOG(ERROR) << "Unable to load program " << id << " dependent shaders are not loaded";
        return false;
      }

      auto v_shader = viter->second;
      auto f_shader = fiter->second;

      auto program = std::make_shared<GL::Program>();
      if (!program->attach(*v_shader, *f_shader)) {
        LOG(ERROR) << "Unable to attach shaders to program";
        return false;
      }

      this->program_map[id] = program;
      return true;
    }
  }  // namespace Resources
}  // namespace ExpGame