#pragma once

namespace Exp
{
  namespace GL
  {
    class EBO
    {
     public:
      EBO();
      ~EBO();

      auto gen() noexcept -> bool;

      auto bind() const noexcept -> bool;

      auto del() noexcept -> bool;

      auto is_valid() const noexcept -> bool;

     private:
      GLuint id;
    };
  }  // namespace GL
}  // namespace Exp