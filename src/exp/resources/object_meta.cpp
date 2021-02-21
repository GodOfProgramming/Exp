#include "object_meta.hpp"

#include "exp/constants.hpp"

namespace Exp
{
  namespace R
  {
    void ObjectMeta::add_usertype(sol::state_view state)
    {
      state.new_usertype<ObjectMeta>(Lua::Usertypes::META_OBJECT, "id", &ObjectMeta::id, "draw_desc", &ObjectMeta::drawdesc);
    }
  }  // namespace R
}  // namespace Exp
