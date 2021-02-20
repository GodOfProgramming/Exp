objects = GameObjects.instance();

meta = nil;
selected_meta = nil;

keys = objects:keys();

lidx = nil;

function get_next_object(index)
  if index < #keys then
    local id = keys[index + 1];
    meta = objects:get(id);
    return true;
  else
    index = 0;
    return false
  end
end

function object_text(self)
  if meta ~= nil then
    self.text = string.format("ID: %s", meta.id);
  else
    self.text = "Nil object detected";
  end

  self.btn_text = "Set";
end

function set_object(self)
  selected_meta = meta;
end

function did_select_object()
  return selected_meta ~= nil;
end

function get_wireframe_value(self)
  self.text = "Wireframe: "..tostring(selected_meta.draw_desc.wireframe);
end
