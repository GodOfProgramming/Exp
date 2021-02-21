#include "model_meta.hpp"

namespace Exp
{
  namespace R
  {
    void ModelMeta::add_usertype(sol::state_view state)
    {
      state.new_usertype<ModelMeta>("ModelMeta", "id", &ModelMeta::id);
    }
  }  // namespace R
}  // namespace Exp
