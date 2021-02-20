index = 1;

objects = GameObjects.instance();

obj = nil;
selected_obj = nil;

keys = objects:keys();

lidx = nil;

function get_next_object(loop_index)
  lidx = loop_index;
  local id = keys[index];

  if id == nil then
    index = 1;
    return false;
  end

  obj = objects:get(id);

  if obj == nil then
    index = 1;
    return false;
  else
    index = index + 1;
    return true;
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
  print(string.format("idx = %d", lidx));
end

function did_select_object()
  return selected_obj ~= nil;
end

function get_wireframe_value(self)
  self.text = "Wireframe: "..tostring(selected_obj.draw_desc.wireframe);
end
