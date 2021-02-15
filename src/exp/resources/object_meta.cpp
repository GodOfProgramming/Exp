#include "object_meta.hpp"

namespace Exp
{
  namespace R
  {
    void ObjectMeta::add_usertype(sol::state& state)
    {
      state.new_usertype<ObjectMeta>("ObjectMeta", "id", &ObjectMeta::id);
    }
  }  // namespace R
}  // namespace Exp
