#pragma once

#include "exp/gl/ebo.hpp"
#include "exp/gl/vao.hpp"
#include "exp/gl/vbo.hpp"

namespace Exp
{
  namespace Resources
  {
    struct ModelMeta
    {
      std::shared_ptr<GL::VAO> vao;
      std::shared_ptr<GL::VBO> vbo;
      std::shared_ptr<GL::EBO> ebo;
    };
  }  // namespace Resources
}  // namespace Exp