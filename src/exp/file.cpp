#include "file.hpp"

namespace ExpGame
{
  File::File(std::string d)
   : data(d)
  {}

  auto File::save(std::filesystem::path path) -> bool
  {
    std::ofstream ostr(path);
  }

  auto File::load(std::filesystem::path path) -> LoadResult
  {
    if (!std::filesystem::exists(path)) {
      std::stringstream ss;
      ss << "unable to load " << path << ", current working directory is " << std::filesystem::current_path();
      return LoadResult::err(ss.str());
    }
    std::string contents;
    std::ifstream istr(path);
    std::istreambuf_iterator<char> input_iter(istr), empty_iter;
    std::back_insert_iterator<std::string> string_inserter(contents);
    std::copy(input_iter, empty_iter, string_inserter);
    return LoadResult::ok(File(contents));
  }
}  // namespace ExpGame
