#include "iresource.hpp"

#include "exp/io/file.hpp"

namespace Exp
{
  namespace R
  {
    void IResource::load_src_file(std::filesystem::path path, std::function<void(const std::string_view&)> callback)
    {
      using nlohmann::json;

      IO::File::load(path, [&](const std::string_view src) { callback(src); });
    }

    void IResource::load_json_file(std::filesystem::path path, std::function<void(const nlohmann::json&)> callback)
    {
      using nlohmann::json;
      IResource::load_src_file(path, [&](const std::string_view& src) {
        json objects;

        try {
          objects = json::parse(src);
        } catch (std::exception& e) {
          LOG(WARNING) << "could not parse json (path = " << path << "): " << e.what();
          return;
        }

        callback(objects);
      });
    }
  }  // namespace R
}  // namespace Exp