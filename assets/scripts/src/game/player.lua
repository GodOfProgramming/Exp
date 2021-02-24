-- player.lua
gl = require ('gl');
geom = require('geom');
input = require('input');
game = require('game');
info = game.info.instance();
camera = game.camera.instance();
keyboard = input.keyboard.instance();

self = nil;

current_action = "run_up";
animation_frame = 0;

u_tex_coords = nil;
u_model = nil;
u_view = nil;
u_proj = nil

location = geom.vec3.new(0, 0, 0);

function construct(player)
  self = player;

  u_tex_coords = gl.uniform.create("u_tex_coords");
  self.uniforms:set("tex_coords", u_tex_coords);

  u_model = gl.uniform.create("u_model");
  self.uniforms:set("model", u_model);

  u_view = gl.uniform.create("u_view");
  self.uniforms:set("view", u_view);

  u_proj = gl.uniform.create("u_projection");
  self.uniforms:set("projection", u_proj);

  local u_tex_ratio = gl.uniform.create("u_tex_ratio");
  self.uniforms:set("tex_ratio", u_tex_ratio);

  u_tex_ratio:set_vec2(self.meta.animation:ratio());
end

function update_animation(anim)
  if current_action ~= anim then
    animation_frame = 0;
    current_action = anim;
  end
end

function update(_)
  if info.state ~= game.state.PLAYING then
    return;
  end

  local uv = self.meta.animation:uv(current_action, animation_frame);
  if info.frames % 15 == 0 then
    animation_frame = self.meta.animation:next(current_action, animation_frame);
  end
  u_tex_coords:set_vec2(uv);

  local prev_loc = geom.vec3.new(location.x, location.y, location.z);

  if keyboard:check(input.key.W) == input.action.PRESS then
    location.y = location.y + 1;
  end

  if keyboard:check(input.key.A) == input.action.PRESS then
    location.x = location.x - 1;
  end

  if keyboard:check(input.key.S) == input.action.PRESS then
    location.y = location.y - 1;
  end

  if keyboard:check(input.key.D) == input.action.PRESS then
    location.x = location.x + 1;
  end

  if prev_loc.y > location.y then
    update_animation("run_down");
  elseif prev_loc.y < location.y then
    update_animation("run_up");
  elseif prev_loc.x > location.x then
    update_animation("run_left");
  elseif prev_loc.x < location.x then
    update_animation("run_right");
  elseif current_action ~= "still_left" and current_action ~= "still_right" and current_action ~= "still_up" and current_action ~= "still_down" then
    if current_action == "run_left" then
      update_animation("still_left")
    elseif current_action == "run_right" then
      update_animation("still_right")
    elseif current_action == "run_up" then
      update_animation("still_up")
    elseif current_action == "run_down" then
      update_animation("still_down")
    end
  end

  camera:move_to(location);

  local transform = geom.mat4.identity();
  transform = transform:translate(location);
  transform = transform:scale(geom.vec3.new(100, 100, 0));
  u_model:set_mat4(transform);
  u_view:set_mat4(camera:get_view());
  u_proj:set_mat4(camera:get_projection());
end