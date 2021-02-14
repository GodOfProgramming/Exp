#include "error_map.hpp"

namespace Exp
{
  namespace GL
  {
    auto ErrorMap::instance() -> ErrorMap&
    {
      static ErrorMap err_map;
      return err_map;
    }

    ErrorMap::ErrorMap()
     : definitions({
        { GL_NO_ERROR, "No error detected" },
        { GL_INVALID_ENUM, "Illegal enumeration" },
        { GL_INVALID_VALUE, "Illegal value parameter" },
        { GL_INVALID_OPERATION, "Wrong parameter for state change" },
        { GL_STACK_OVERFLOW, "Stack push overflow" },
        { GL_STACK_UNDERFLOW, "Stack pop on empty stack" },
        { GL_OUT_OF_MEMORY, "Not enough memory for given operation" },
        { GL_INVALID_FRAMEBUFFER_OPERATION, "Read/write to framebuffer that is not complete" },
       })
    {}

    auto ErrorMap::check(const char* file, int line) noexcept -> bool
    {
      auto err = glGetError();
      if (err == GL_NO_ERROR) {
        return true;
      }

      auto entry = this->errors.find(err);
      if (entry == this->errors.end()) {
        this->errors[err].occurrences[std::string(file)][line]++;
      } else {
        auto& entry = this->errors[err];
        auto desc   = this->definitions.find(err);
        if (desc == this->definitions.end()) {
          entry.desc = std::to_string(err);
        } else {
          entry.desc = desc->second;
        }
        entry.occurrences[std::string(file)][line]++;
      }
      return false;
    }

    auto ErrorMap::begin() const noexcept -> Iter
    {
      return this->errors.begin();
    }

    auto ErrorMap::end() const noexcept -> Iter
    {
      return this->errors.end();
    }

    auto ErrorMap::error_count() const noexcept -> std::size_t
    {
      return this->errors.size();
    }
  }  // namespace GL
}  // namespace Exp
