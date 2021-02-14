#pragma once

#include "shader.hpp"

namespace Exp
{
  namespace GL
  {
    class Program
    {
     public:
      using ProgramID = GLuint;
      Program();
      ~Program();

      auto attach(const Shader& shaders) -> bool;

      auto link(std::string& errstr) -> bool;

      auto program_id() const noexcept -> ProgramID;

      auto is_valid() const noexcept -> bool;

      auto use() -> bool;

     private:
      ProgramID id;
      bool valid;
    };
  }  // namespace GL
}  // namespace Exp
