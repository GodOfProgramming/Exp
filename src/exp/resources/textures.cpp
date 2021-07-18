#include "textures.hpp"

#include "exp/constants.hpp"
#include "exp/io.hpp"
#include "exp/io/file.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

namespace Exp
{
  namespace R
  {
    void Textures::load_all(World& world)
    {
      LOG(INFO) << "loading textures";
      IO::iterate_dir_with_namespace(Cfg::Dir::TEXTURES, std::string{ "exp" }, [&](const std::filesystem::path path, const std::string& nspace) {
        using nlohmann::json;
        IResource::load_json_file(path, [&](const json& objects) {
          if (!objects.is_object()) {
            LOG(WARNING) << "object json is not in proper format, first type is not object";
            return;
          }

          for (const auto item : objects.items()) {
            auto id  = nspace + "." + item.key();
            auto obj = item.value();

            LOG(INFO) << "loading texture " << id;

            // TODO parse this
            GL::TextureDescriptor descriptor;

            std::string filename;
            if (!TextureMeta::has_file(obj, filename)) {
              continue;
            }

            std::stringstream ss;
            ss << Assets::Dir::TEXTURES << '/' << filename;
            std::string fn = ss.str();

            int width, height, num_chans;
            auto* dat = stbi_load(fn.c_str(), &width, &height, &num_chans, 0);
            if (dat == nullptr) {
              LOG(WARNING) << "could not load image " << id;
              continue;
            }

            std::shared_ptr<uint8_t> data(dat);

            auto tex = std::make_shared<GL::Texture>();
            if (!tex->gen()) {
              LOG(WARNING) << "could not generate texture";
              continue;
            }

            if (!tex->bind()) {
              LOG(WARNING) << "could not bind texture";
              continue;
            }

            if (!tex->configure(descriptor, width, height, data)) {
              continue;
            }

            auto meta        = std::make_shared<TextureMeta>();
            meta->width      = width;
            meta->height     = height;
            meta->img        = data;
            meta->tex        = tex;
            meta->descriptor = descriptor;
            meta->file       = filename;

            this->textures.emplace(id, meta);
          }
        });
      });
    }

    void Textures::release()
    {
      this->textures.clear();
    }

    auto Textures::find(std::string id) const noexcept -> TextureMap::const_iterator
    {
      return this->textures.find(id);
    }

    auto Textures::begin() const noexcept -> TextureMap::const_iterator
    {
      return this->textures.begin();
    }

    auto Textures::end() const noexcept -> TextureMap::const_iterator
    {
      return this->textures.end();
    }

  }  // namespace R
}  // namespace Exp
