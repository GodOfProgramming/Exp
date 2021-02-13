#include "io.hpp"

namespace ExpGame
{
  namespace IO
  {
    File::File(std::string d)
     : data(d)
    {}

    auto File::save(std::filesystem::path path) -> bool
    {
      std::ofstream ostr(path);
      return true;
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

    auto File::dirname(std::filesystem::path path) -> std::filesystem::path
    {
      return path.parent_path();
    }

    void iterate_dir_with_namespace(
     std::filesystem::path dir,
     std::string nspace,
     const std::function<void(const std::filesystem::path, const std::string&)> fn)
    {
      for (const auto& iter : std::filesystem::directory_iterator(dir)) {
        if (iter.is_directory()) {
          iterate_dir_with_namespace(iter, nspace + "." + iter.path().filename().string(), fn);
        } else if (iter.is_regular_file()) {
          nspace += "." + iter.path().stem().string();
          fn(iter, nspace);
        }
      }
    }
  }  // namespace IO
}  // namespace ExpGame
