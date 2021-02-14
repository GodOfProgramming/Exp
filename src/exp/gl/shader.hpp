#pragma once

namespace Exp
{
  namespace GL
  {
    class Shader
    {
     public:
      enum class Type
      {
        VERTEX   = GL_VERTEX_SHADER,
        FRAGMENT = GL_FRAGMENT_SHADER,
      };

      using ShaderID = GLuint;

      Shader() noexcept;

      ~Shader() noexcept;

      auto compile(Type t, const std::string& src, std::string& errstr) noexcept -> bool;

      auto shader_id() const noexcept -> ShaderID;

      auto is_valid() const noexcept -> bool;

     private:
      bool valid;
      ShaderID id;
    };
  }  // namespace GL
}  // namespace Exp
