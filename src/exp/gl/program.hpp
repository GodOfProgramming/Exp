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
      Program(const std::string id);
      ~Program();

      static void add_usertype(sol::state_view state);

      auto attach(const Shader& shaders) -> bool;

      auto link(std::string& errstr) -> bool;

      auto program_id() const noexcept -> ProgramID;

      auto is_valid() const noexcept -> bool;

      auto use() -> bool;

      const std::string id;

     private:
      ProgramID pid;
      bool valid;
    };
  }  // namespace GL
}  // namespace Exp
