#include "file.hpp"

namespace Exp
{
  namespace IO
  {
    File::File(std::string d)
     : data(d)
    {}

    auto File::save(std::filesystem::path path) -> bool
    {
      std::ofstream ostr(path);
      if (!ostr) {
        return false;
      }

      ostr << this->data;

      ostr.close();
      if (!ostr) {
        return false;
      }
      return true;
    }

    void File::load(std::filesystem::path path, std::function<void(std::string_view)> fn)
    {
      if (!std::filesystem::exists(path)) {
        return;
      }
      std::string contents;
      std::ifstream istr(path);
      std::istreambuf_iterator<char> input_iter(istr), empty_iter;
      std::back_insert_iterator<std::string> string_inserter(contents);
      std::copy(input_iter, empty_iter, string_inserter);
      fn(contents);
    }

    auto File::dirname(std::filesystem::path path) -> std::filesystem::path
    {
      return path.parent_path();
    }
  }  // namespace IO
}  // namespace Exp