-- square.lua
gl = require('gl');
geom = require('geom');
game = require('game');
info = game.info.instance();
camera = game.camera.instance();

function construct(obj)
  local u_colors = gl.uniform.create("u_colors");
  obj.uniforms:set("colors", u_colors);

  local u_model = gl.uniform.create("u_model");
  obj.uniforms:set("model", u_model);

  local u_view = gl.uniform.create("u_view");
  obj.uniforms:set("view", u_view);

  local u_proj = gl.uniform.create("u_projection");
  obj.uniforms:set("projection", u_proj);

  obj.data = {
    uniforms = {
      colors = u_colors,
      model = u_model,
      view = u_view,
      proj = u_proj,
    }
  };
end

function update(obj)
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

  local uniforms = obj.data.uniforms;

  uniforms.colors:set_v_vec3(colors);

  local size = 400;

  local transform = geom.mat4.identity();
  transform = transform:translate(geom.vec3.new(size, size / 2, 0));
  transform = transform:scale(geom.vec3.new(size, size, 0));
  uniforms.model:set_mat4(transform);
  uniforms.view:set_mat4(camera:get_view());
  uniforms.proj:set_mat4(camera:get_projection());
end