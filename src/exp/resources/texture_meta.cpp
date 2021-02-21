#include "texture_meta.hpp"

#include "exp/constants.hpp"

namespace Exp
{
  namespace R
  {
    auto TextureMeta::has_file(const nlohmann::json& json, std::string& value) -> bool
    {
      auto file_json = json[JSON::Keys::TEXTURE_FILE];
      if (file_json.is_string()) {
        value = file_json;
      } else {
        LOG(WARNING) << "object missing construct fn";
        return false;
      }

      return true;
    }
  }  // namespace R
}  // namespace Exp