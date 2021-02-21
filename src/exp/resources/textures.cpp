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
      IO::iterate_dir_with_namespace(Cfg::Dir::TEXTURES, std::string{ "exp" }, [&](const std::filesystem::path path, const std::string& nspace) {
        using nlohmann::json;
        this->load_json_file(path, [&](const json& objects) {
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

            auto img = std::make_shared<cimg_library::CImg<uint8_t>>();
            img->load(fn.c_str());

            auto tex = std::make_shared<GL::Texture>();
            if (!tex->gen()) {
              LOG(WARNING) << "could not generate texture";
              continue;
            }

            if (!tex->bind()) {
              LOG(WARNING) << "could not bind texture";
              continue;
            }

            if (!tex->configure(descriptor, img)) {
              continue;
            }

            auto meta        = std::make_shared<TextureMeta>();
            meta->file       = filename;
            meta->descriptor = descriptor;
            meta->img        = img;
            meta->tex        = tex;

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
