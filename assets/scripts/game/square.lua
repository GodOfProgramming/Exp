geom = require('geom');
gl = require ('gl');
game = require('game');

info = game.info.instance();

self = nil;

function construct(square)
  self = square;
  self.uniforms:set("colors", gl.uniform.new("u_colors"));
end

function update()
  local scale = 0.05
  local r = math.sin(info.frames * scale) / 2.0 + 0.5
  local g = math.sin(info.frames * scale) / 2.0 + 0.5
  local b = math.sin(info.frames * scale) / 2.0 + 0.5
  local rgb = math.sin(r + g + b) / 2.0 + 0.5

  local colors = {
    geom.vec3.new(r, 0, 0),
    geom.vec3.new(0, g, 0),
    geom.vec3.new(0, 0, b),
    geom.vec3.new(rgb, rgb, rgb),
  };

  local uniform = self.uniforms:get("colors");
  uniform:set_v_vec3(colors);
end