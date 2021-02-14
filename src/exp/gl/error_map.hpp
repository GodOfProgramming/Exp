#pragma once

#include "error.hpp"

namespace Exp
{
  namespace GL
  {
    class ErrorMap
    {
      ErrorMap();

      using Internal = std::map<GLenum, Error>;
      using Iter     = Internal::const_iterator;

     public:
      ErrorMap(const ErrorMap&) = delete;
      ErrorMap(ErrorMap&&)      = delete;
      auto operator=(const ErrorMap&) -> ErrorMap& = delete;
      auto operator=(ErrorMap&&) -> ErrorMap& = delete;

      static auto instance() -> ErrorMap&;

      auto check(const char* file, int line) noexcept -> bool;

      auto begin() const noexcept -> Iter;
      auto end() const noexcept -> Iter;

      auto error_count() const noexcept -> std::size_t;

     private:
      Internal errors;
      const std::map<GLenum, std::string> definitions;
    };
  }  // namespace GL
}  // namespace Exp

#define GL_CHECK() Exp::GL::ErrorMap::instance().check(__FILE__, __LINE__)
