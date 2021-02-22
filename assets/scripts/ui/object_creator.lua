r = require('r');
objects = r.game_objects.instance();

meta = nil;
selected_meta = nil;

keys = objects:keys();

lidx = nil;

window = nil;

function save_window(self)
  window = self;
end

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
end

function set_object(self)
  window.title = meta.id;
  selected_meta = meta;
end

function did_select_object()
  return selected_meta ~= nil;
end

function get_shader_id(self)
  self.text = "Shader: "..selected_meta.shader_id;
end

function get_model_id(self)
  self.text = "Model: "..selected_meta.model_id;
end

function get_animation_id(self)
  self.text = "Animation: "..selected_meta.animation_id;
end

function get_script_id(self)
  self.text = "Script: "..selected_meta.script_id;
end

function get_construct_fn(self)
  self.text = "On Construct: "..selected_meta.construct_fn;
end

function get_update_fn(self)
  self.text = "On Update: "..selected_meta.update_fn;
end

function get_wireframe_value(self)
  self.text = "Wireframe: "..tostring(selected_meta.draw_desc.wireframe);
end
