objects = GameObjects.instance();

obj = nil;
selected_obj = nil;

keys = objects:keys();

lidx = nil;

function get_next_object(index)
  if index < #keys then
    local id = keys[index + 1];
    obj = objects:get(id);
    return true;
  else
    index = 0;
    return false
  end
end

function object_text(self)
  if obj ~= nil then
    self.text = string.format("ID: %s", obj.id);
  else
    self.text = "Nil object detected";
  end

  self.btn_text = "Set";
end

function set_object(self)
  selected_obj = obj;
end

function did_select_object()
  return selected_obj ~= nil;
end

function get_wireframe_value(self)
  self.text = "Wireframe: "..tostring(selected_obj.draw_desc.wireframe);
end
