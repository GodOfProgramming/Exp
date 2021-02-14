function Instantiate(self)
  self.uniforms:set("color", Uniform.new("u_color"));
end

function Update(self)
  local green_value = math.sin(GameInfo.frames * 0.1) / 2.0 + 0.5
  local uniform = self.uniforms:get("color");
  uniform:set_vec4(0.0, green_value, 0.0, 1.0);
end