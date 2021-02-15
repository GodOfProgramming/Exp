function Instantiate(self)
  self.uniforms:set("colors", Uniform.new("u_colors"));
end

function Update(self)
  local scale = 0.05
  local r = math.sin(GameInfo.frames * scale) / 2.0 + 0.5
  local g = math.sin(GameInfo.frames * scale) / 2.0 + 0.5
  local b = math.sin(GameInfo.frames * scale) / 2.0 + 0.5
  local rgb = math.sin(r + g + b) / 2.0 + 0.5

  local colors = {
    Vec3.new(r, 0, 0),
    Vec3.new(0, g, 0),
    Vec3.new(0, 0, b),
    Vec3.new(rgb, rgb, rgb),
  };

  local uniform = self.uniforms:get("colors");
  uniform:set_v_vec3(colors);
end