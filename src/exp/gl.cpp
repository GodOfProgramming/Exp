#include "gl.hpp"

namespace ExpGame
{
  namespace GL
  {
    auto VBO::gen() -> bool
    {
      glGenBuffers(1, &this->id);
      return GL_CHECK();
    }

    auto VBO::bind() -> bool
    {
      glBindBuffer(GL_ARRAY_BUFFER, this->id);
      return GL_CHECK();
    }

    auto ErrorMap::instance() -> ErrorMap&
    {
      static ErrorMap err_map;
      return err_map;
    }

    ErrorMap::ErrorMap() {}

    auto ErrorMap::check(const char* file, int line) -> bool
    {
      auto err = glGetError();
      if (err == GL_NO_ERROR) {
        return true;
      } else {
        auto entry = this->errors.find(err);
        if (entry == this->errors.end()) {
          entry->second.occurrences[std::string(file)].push_back(line);
        } else {
          auto& entry = this->errors[err];
          entry.desc  = std::to_string(err);
          entry.occurrences[std::string(file)].push_back(line);
        }
        return false;
      }
    }

    auto ErrorMap::begin() -> Iter
    {
      return this->errors.begin();
    }

    auto ErrorMap::end() -> Iter
    {
      return this->errors.end();
    }
  }  // namespace GL
}  // namespace ExpGame
