-- square.lua
gl = require('gl');
geom = require('geom');
game = require('game');
info = game.info.instance();
camera = game.camera.instance();

u_colors = nil;
u_model = nil;
u_view = nil;
u_proj = nil

self = nil;

function construct(square)
  self = square;
  u_colors = gl.uniform.create("u_colors");
  self.uniforms:set("colors", u_colors);

  u_model = gl.uniform.create("u_model");
  self.uniforms:set("model", u_model);

  u_view = gl.uniform.create("u_view");
  self.uniforms:set("view", u_view);

  u_proj = gl.uniform.create("u_projection");
  self.uniforms:set("projection", u_proj);
end

function update(_)
  if info.state ~= game.state.PLAYING then
    return;
  end

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

  u_colors:set_v_vec3(colors);

  local size = 400;

  local transform = geom.mat4.identity();
  transform = transform:translate(geom.vec3.new(size, size / 2, 0));
  transform = transform:scale(geom.vec3.new(size, size, 0));
  u_model:set_mat4(transform);
  u_view:set_mat4(camera:get_view());
  u_proj:set_mat4(camera:get_projection());
end