#include "models.hpp"

#include "exp/constants.hpp"
#include "exp/io.hpp"
#include "exp/io/file.hpp"
#include "exp/math/vertex.hpp"

namespace Exp
{
  namespace R
  {
    auto Models::instance() noexcept -> Models&
    {
      static Models models;
      return models;
    }

    void Models::load_all()
    {
      LOG(INFO) << "loading models";
      IO::iterate_dir_with_namespace(CFG_DIR_GAME_MODELS, std::string{ "exp" }, [&](const std::filesystem::path path, const std::string& nspace) {
        using nlohmann::json;
        this->load_json_file(path, [&](const json& objects) {
          if (!objects.is_object()) {
            LOG(WARNING) << "object json is not in proper format, first type is not object";
            return;
          }

          for (const auto item : objects.items()) {
            auto id  = nspace + "." + item.key();
            auto obj = item.value();

            LOG(INFO) << "loading model " << id;

            auto vbo = std::make_shared<GL::VBO>();

            if (!vbo->gen()) {
              LOG(WARNING) << "could not generate vbo";
              continue;
            }

            if (!vbo->is_valid()) {
              LOG(WARNING) << "unable to load object, vbo not valid";
              continue;
            }

            std::optional<std::vector<Math::Vertex>> vertices_container;

            auto vertices_json = obj["vertices"];
            if (vertices_json.is_object()) {
              auto point_json  = vertices_json["points"];
              auto normal_json = vertices_json["normals"];
              auto uv_json     = vertices_json["uvs"];

              if (!point_json.is_array() || !normal_json.is_array() || !uv_json.is_array()) {
                LOG(WARNING) << "could not load model, vertex components are not all arrays";
                continue;
              }

              std::vector<float> point_list  = point_json;
              std::vector<float> normal_list = normal_json;
              std::vector<float> uv_list     = uv_json;

              if (point_list.size() % 3 != 0 || point_list.size() != normal_list.size()) {
                LOG(WARNING) << "could not load model, point/normal length out of sync";
                continue;
              }

              auto vertex_count = point_list.size() / 3;

              if (uv_list.size() != vertex_count * 2) {
                LOG(WARNING) << "could not load model, uv size out of sync";
                continue;
              }

              std::vector<Math::Vertex> vertices(vertex_count);

              for (std::size_t i = 0; i < vertex_count; i++) {
                auto& vertex = vertices[i];
                auto& pt     = vertex.position;
                pt.x         = point_list[i * 3 + 0];
                pt.y         = point_list[i * 3 + 1];
                pt.z         = point_list[i * 3 + 2];
                auto& nrm    = vertex.normal;
                nrm.x        = normal_list[i * 3 + 0];
                nrm.y        = normal_list[i * 3 + 1];
                nrm.z        = normal_list[i * 3 + 2];
                auto& uv     = vertex.uv;
                uv.x         = uv_list[i * 2 + 0];
                uv.y         = uv_list[i * 2 + 1];
              }

              vertices_container = std::move(vertices);
            } else {
              LOG(WARNING) << "no way to create object vertices";
              continue;
            }

            auto ebo = std::make_shared<GL::EBO>();

            if (!ebo->gen()) {
              LOG(WARNING) << "could not generate ebo";
              continue;
            }

            if (!ebo->is_valid()) {
              LOG(WARNING) << "unable to load object, ebo not valid";
              continue;
            }

            std::optional<std::vector<GLuint>> indices_container;

            if (obj["indices"].is_array()) {
              std::vector<GLuint> list = obj["indices"];
              indices_container        = std::move(list);
            } else {
              LOG(WARNING) << "no way to create object indices";
            }

            auto vao = std::make_shared<GL::VAO>(vbo, ebo);

            if (!vao->gen()) {
              LOG(WARNING) << "unable to generate vao";
              continue;
            }

            if (!vertices_container.has_value()) {
              LOG(WARNING) << "no vertices to make object out of";
              continue;
            }

            if (!indices_container.has_value()) {
              LOG(WARNING) << "no indices to make object out of";
              continue;
            }

            std::vector<Math::Vertex> vertices = std::move(vertices_container.value());
            std::vector<GLuint> indices        = std::move(indices_container.value());

            if (!vao->set<GL::GlDraw::STATIC>(vertices, indices)) {
              LOG(WARNING) << "unable to set vertices to object";
              continue;
            }

            if (!vao->is_valid()) {
              LOG(WARNING) << "unable to load object, vao not valid";
              continue;
            }

            auto meta = std::make_shared<ModelMeta>();
            meta->vao = vao;
            meta->vbo = vbo;
            meta->ebo = ebo;

            this->models.emplace(id, meta);
          }
        });
      });
    }

    void Models::release()
    {
      this->models.clear();
    }

    auto Models::find(std::string id) const noexcept -> ModelMap::const_iterator
    {
      return this->models.find(id);
    }

    auto Models::begin() const noexcept -> ModelMap::const_iterator
    {
      return this->models.begin();
    }

    auto Models::end() const noexcept -> ModelMap::const_iterator
    {
      return this->models.end();
    }
  }  // namespace R
}  // namespace Exp