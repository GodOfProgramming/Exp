#include "shader_program_meta.hpp"

#include "exp/constants.hpp"
#include "exp/io/file.hpp"

namespace Exp
{
  namespace R
  {
    ShaderProgramMeta::ShaderProgramMeta(std::string st, json& sj)
     : type(st)
    {
      if (!sj.is_object()) {
        LOG(WARNING) << "shader is not object, skipping: " << sj.dump(2);
        return;
      }

      for (const auto& item : sj.items()) {
        auto shader_type    = item.key();
        auto filename_value = item.value();

        if (!filename_value.is_string()) {
          LOG(INFO) << "could not load shader: " << filename_value.dump(2);
          continue;
        }

        ShaderMeta* shader = nullptr;

        if (shader_type == JSON::Keys::SHADER_VERTEX) {
          if (!this->vertex.file.empty()) {
            LOG(WARNING) << "program already has vertex shader linked: " << this->type;
            continue;
          }

          shader = &this->vertex;
        } else if (shader_type == JSON::Keys::SHADER_FRAGMENT) {
          if (!this->fragment.file.empty()) {
            LOG(WARNING) << "program already has fragment shader linked: " << this->type;
            continue;
          }

          shader = &this->fragment;
        } else {
          continue;
        }

        shader->file  = filename_value;
        auto abs_path = std::string(Assets::Dir::SHADER_OUTPUT) + "/" + shader->file;
        bool loaded   = false;

        IO::File::load(abs_path, [&](const std::string_view& src) {
          shader->source = src;
          loaded         = true;
        });

        if (!loaded) {
          LOG(WARNING) << "unable to load shader source";
          continue;
        }

        shader->present = true;
      }
    }

    auto ShaderProgramMeta::is_valid() const noexcept -> bool
    {
      return !this->type.empty() && this->has_vertex() && this->has_fragment();
    }

    auto ShaderProgramMeta::has_vertex() const noexcept -> bool
    {
      return this->vertex.present;
    }

    auto ShaderProgramMeta::has_fragment() const noexcept -> bool
    {
      return this->fragment.present;
    }
  }  // namespace R
}  // namespace Exp