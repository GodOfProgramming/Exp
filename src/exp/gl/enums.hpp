#pragma once

namespace Exp
{
  namespace GL
  {
    enum class GlDraw
    {
      STREAM  = GL_STREAM_DRAW,
      STATIC  = GL_STATIC_DRAW,
      DYNAMIC = GL_DYNAMIC_DRAW,
    };

    enum class TextureWrap
    {
      REPEAT = GL_REPEAT,
    };

    enum class TextureFilter
    {
      LINEAR = GL_LINEAR,
    };
  }  // namespace GL
}  // namespace Exp
