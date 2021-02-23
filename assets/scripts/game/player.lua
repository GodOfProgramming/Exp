inspect = require('inspect');
gl = require ('gl');
geom = require('geom');
game = require('game');
info = game.info.instance();
camera = game.camera.instance();

self = nil;

current_action = "run_up";
animation_frame = 0;

u_tex_coords = nil;
u_transform = nil;
u_view = nil;
u_proj = nil

location = geom.vec3.new();

function construct(player)
  self = player;

  u_proj = gl.uniform.create("u_projection");
  self.uniforms:set("projection", u_proj);

  u_view = gl.uniform.create("u_view");
  self.uniforms:set("view", u_view);

  u_tex_coords = gl.uniform.create("u_tex_coords");
  self.uniforms:set("tex_coords", u_tex_coords);

  local u_tex_ratio = gl.uniform.create("u_tex_ratio");
  self.uniforms:set("tex_ratio", u_tex_ratio);

  u_transform = gl.uniform.create("u_model");
  self.uniforms:set("model", u_transform);

  u_tex_ratio:set_vec2(self.meta.animation:ratio());
end

function update(_)
  if info.state ~= game.state.PLAYING then
    return;
  end

  local uv = self.meta.animation:uv(current_action, animation_frame);
  if info.frames % 15 == 0 then
    animation_frame = self.meta.animation:next(current_action, animation_frame);
    if animation_frame == 0 then
      if current_action == "run_down" then
        current_action = "run_up";
      else
        current_action = "run_down"
      end
    end
  end
  u_tex_coords:set_vec2(uv);

  camera:move_to(location);

  local transform = geom.mat4.identity();
  transform = transform:translate(location);
  transform = transform:scale(geom.vec3.new(100, 100, 0));
  u_transform:set_mat4(transform);
  u_view:set_mat4(camera:get_view());
  u_proj:set_mat4(camera:get_projection());
end