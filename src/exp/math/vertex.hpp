#pragma once

namespace Exp
{
  namespace Math
  {
    struct Vertex
    {
      glm::vec3 position;
      glm::vec3 normal;
      glm::vec2 uv;

      using pos_t  = decltype(Vertex::position);
      using norm_t = decltype(Vertex::normal);
      using uv_t   = decltype(Vertex::uv);
    };
  }  // namespace Math
}  // namespace Exp