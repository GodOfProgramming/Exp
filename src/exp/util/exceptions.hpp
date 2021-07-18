#include <exception>
#include <string>
#include <string_view>

namespace Exp
{
  namespace Util
  {
    class DoNotUseException: public std::exception
    {
     public:
      DoNotUseException(const std::string_view& f)
       : msg{ std::string(f) + " should not be used" }
      {}

      auto what() const noexcept -> const char* override
      {
        return this->msg.c_str();
      }

     private:
      std::string msg;
    };
  }  // namespace Util
}  // namespace Exp