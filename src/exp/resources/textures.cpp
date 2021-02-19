#include "textures.hpp"

#include "exp/constants.hpp"
#include "exp/io.hpp"
#include "exp/io/file.hpp"

namespace Exp
{
  namespace R
  {
    auto Textures::instance() noexcept -> Textures&
    {
      static Textures textures;
      return textures;
    }

    void Textures::load_all()
    {
      LOG(INFO) << "loading textures";
      IO::iterate_dir_with_namespace(CFG_DIR_TEXTURES, std::string{ "exp" }, [&](const std::filesystem::path path, const std::string& nspace) {
        using nlohmann::json;
        this->load_json_file(path, [&](const json& objects) {
          if (!objects.is_object()) {
            LOG(WARNING) << "object json is not in proper format, first type is not object";
            return;
          }

          for (const auto item : objects.items()) {
            auto id  = nspace + "." + item.key();
            auto tex = item.value();

            LOG(INFO) << "loading texture " << id;

            std::string filename;
            auto file_json = tex["file"];
            if (file_json.is_string()) {
              filename = file_json;
            } else if (!file_json.is_null()) {
              LOG(WARNING) << "detected file on texture but was not of type string";
              continue;
            } else {
              LOG(WARNING) << "missing file attribute";
              continue;
            }

            std::stringstream ss;
            ss << DIR_TEXTURES << '/' << filename;
            std::string fn = ss.str();

            auto meta = std::make_shared<TextureMeta>();
            meta->tex.load(fn.c_str());

            this->textures.emplace(id, meta);
          }
        });
      });
    }

    void Textures::release()
    {
      this->textures.clear();
    }
  }  // namespace R
}  // namespace Exp
