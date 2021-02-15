#pragma once

#include "shader_meta.hpp"

namespace Exp
{
  namespace R
  {
    class ShaderProgramMeta
    {
      using json = nlohmann::json;

     public:
      ShaderProgramMeta() = default;
      ShaderProgramMeta(std::string st, json& sj);

      auto is_valid() const noexcept -> bool;

      auto has_vertex() const noexcept -> bool;

      auto has_fragment() const noexcept -> bool;

      std::string type;
      std::string link_error;
      ShaderMeta vertex;
      ShaderMeta fragment;
      std::set<std::string> uniforms;
    };
  }  // namespace R
}  // namespace Exp