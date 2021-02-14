#include "object_meta.hpp"

namespace Exp
{
  namespace Resources
  {
    void ObjectMeta::add_usertype(sol::state& state)
    {
      state.new_usertype<ObjectMeta>("ObjectMeta", "id", &ObjectMeta::id);
    }
  }  // namespace Resources
}  // namespace Exp
