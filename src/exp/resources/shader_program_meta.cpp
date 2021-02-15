#include "shader_program_meta.hpp"

#include "exp/constants.hpp"
#include "exp/io/file.hpp"

namespace Exp
{
  namespace Resources
  {
    ShaderProgramMeta::ShaderProgramMeta(std::string st, json& sj)
     : type(st)
    {
      static const std::set<std::string> KEYS_TO_SKIP = {
        JSON_KEY_SHADER_UNIFORM,
      };

      if (!sj.is_object()) {
        LOG(WARNING) << "shader is not object, skipping: " << sj.dump(2);
        return;
      }

      for (const auto& item : sj.items()) {
        auto shader_type    = item.key();
        auto filename_value = item.value();

        if (KEYS_TO_SKIP.contains(shader_type)) {
          continue;
        }

        if (!filename_value.is_string()) {
          LOG(INFO) << "could not load shader: " << filename_value.dump(2);
          continue;
        }

        ShaderMeta* shader = nullptr;

        if (shader_type == JSON_KEY_SHADER_VERTEX) {
          if (!this->vertex.file.empty()) {
            LOG(WARNING) << "program already has vertex shader linked: " << this->type;
            continue;
          }

          shader = &this->vertex;
        } else if (shader_type == JSON_KEY_SHADER_FRAGMENT) {
          if (!this->fragment.file.empty()) {
            LOG(WARNING) << "program already has fragment shader linked: " << this->type;
            continue;
          }

          shader = &this->fragment;
        } else {
          continue;
        }

        shader->file  = filename_value;
        auto abs_path = std::string(DIR_SHADER_OUTPUT) + "/" + shader->file;
        auto src_res  = IO::File::load(abs_path);
        if (!src_res) {
          LOG(ERROR) << "unable to load shader: " << src_res.err_val();
          continue;
        }

        auto src_file = src_res.ok_val();

        shader->source  = src_file.data;
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
  }  // namespace Resources
}  // namespace Exp