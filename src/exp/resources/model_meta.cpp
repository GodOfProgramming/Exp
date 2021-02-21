#include "model_meta.hpp"

#include "exp/constants.hpp"

namespace Exp
{
  namespace R
  {
    void ModelMeta::add_usertype(sol::state_view state)
    {
      state.new_usertype<ModelMeta>(Lua::Usertypes::META_MODEL, "id", &ModelMeta::id);
    }
  }  // namespace R
}  // namespace Exp
