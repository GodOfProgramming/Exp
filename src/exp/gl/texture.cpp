#include "texture.hpp"

#include "error_map.hpp"

namespace Exp
{
  namespace GL
  {
    auto Texture::gen() noexcept -> bool
    {
      glGenTextures(1, &this->id);
      return GL_CHECK();
    }

    auto Texture::bind() const noexcept -> bool
    {
      glBindTexture(GL_TEXTURE_2D, this->id);
      return GL_CHECK();
    }

    auto Texture::configure(const TextureDescriptor desc, int width, int height, std::shared_ptr<uint8_t> img) noexcept -> bool
    {
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, static_cast<GLint>(desc.wrap_s));
      if (!GL_CHECK()) {
        LOG(WARNING) << "could not set wrap s on texture";
        return false;
      }

      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, static_cast<GLint>(desc.wrap_t));
      if (!GL_CHECK()) {
        LOG(WARNING) << "could not set wrap t on texture";
        return false;
      }

      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, static_cast<GLint>(desc.min));
      if (!GL_CHECK()) {
        LOG(WARNING) << "could not set min filter on texture";
        return false;
      }

      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, static_cast<GLint>(desc.mag));
      if (!GL_CHECK()) {
        LOG(WARNING) << "could not set mag filter on texture";
        return false;
      }

      glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, img.get());
      if (!GL_CHECK()) {
        LOG(WARNING) << "cannot create texture from image";
        return false;
      }

      glGenerateMipmap(GL_TEXTURE_2D);
      if (!GL_CHECK()) {
        LOG(WARNING) << "cannot generate mipmaps";
        return false;
      }

      return this->valid = true;
    }

    auto Texture::is_valid() const noexcept -> bool
    {
      return this->valid;
    }
  }  // namespace GL
}  // namespace Exp