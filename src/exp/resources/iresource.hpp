#pragma once

namespace Exp
{
  namespace R
  {
    class IResource
    {
     public:
      virtual void load_all() = 0;
      virtual void release()  = 0;

     protected:
      void load_src_file(std::filesystem::path path, std::function<void(const std::string_view&)> callback);
      void load_json_file(std::filesystem::path path, std::function<void(const nlohmann::json&)> callback);
    };
  }  // namespace R
}  // namespace Exp