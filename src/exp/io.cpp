#include "io.hpp"

namespace Exp
{
  namespace IO
  {
    void iterate_dir_with_namespace(
     std::filesystem::path dir, std::string nspace, const std::function<void(const std::filesystem::path, const std::string&)> fn)
    {
      for (const auto& iter : std::filesystem::directory_iterator(dir)) {
        if (iter.is_directory()) {
          iterate_dir_with_namespace(iter, nspace + "." + iter.path().filename().string(), fn);
        } else if (iter.is_regular_file()) {
          fn(iter, nspace + "." + iter.path().stem().string());
        }
      }
    }
  }  // namespace IO
}  // namespace Exp
