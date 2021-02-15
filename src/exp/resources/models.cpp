#include "models.hpp"

#include "exp/constants.hpp"
#include "exp/io.hpp"
#include "exp/io/file.hpp"

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
      IO::iterate_dir_with_namespace(CFG_DIR_GAME_MODELS, std::string{ "exp" }, [&](const std::filesystem::path path, const std::string& nspace) {
        using nlohmann::json;

        auto file_res = IO::File::load(path);
        if (!file_res) {
          LOG(WARNING) << "unable to load model configuration file: " << file_res.err_val();
          return;
        }

        auto file = file_res.ok_val();

        json objects;

        try {
          objects = json::parse(file.data);
        } catch (std::exception& e) {
          LOG(WARNING) << "could not parse json (path = " << path << "): " << e.what();
          return;
        }

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

          std::optional<std::vector<glm::vec3>> vertices_container;

          if (obj["vertices"].is_array()) {
            std::vector<float> list = obj["vertices"];
            if (list.size() % 3 != 0) {
              LOG(WARNING) << "cannot load item " << id << ", invalid number of vertices";
              continue;
            }
            std::vector<glm::vec3> vertices(list.size() / 3);

            for (std::size_t i = 0; i < list.size(); i += 3) {
              glm::vec3 vertex{};
              vertex.x        = list[i + 0];
              vertex.y        = list[i + 1];
              vertex.z        = list[i + 2];
              vertices[i / 3] = vertex;
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

          std::vector<glm::vec3> vertices = std::move(vertices_container.value());
          std::vector<GLuint> indices     = std::move(indices_container.value());

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