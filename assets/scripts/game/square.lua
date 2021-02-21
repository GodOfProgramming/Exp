function construct()
  self.uniforms:set("colors", Uniform.new("u_colors"));
end

function update()
  local scale = 0.05
  local r = math.sin(game_info.frames * scale) / 2.0 + 0.5
  local g = math.sin(game_info.frames * scale) / 2.0 + 0.5
  local b = math.sin(game_info.frames * scale) / 2.0 + 0.5
  local rgb = math.sin(r + g + b) / 2.0 + 0.5

  local colors = {
    vec3.new(r, 0, 0),
    vec3.new(0, g, 0),
    vec3.new(0, 0, b),
    vec3.new(rgb, rgb, rgb),
  };

  local uniform = self.uniforms:get("colors");
  uniform:set_v_vec3(colors);
end