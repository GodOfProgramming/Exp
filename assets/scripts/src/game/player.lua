-- player.lua
gl = require ('gl');
geom = require('geom');
input = require('input');
game = require('game');
info = game.info.instance();
camera = game.camera.instance();
keyboard = input.keyboard.instance();

function construct(obj)
  u_tex_coords = gl.uniform.create("u_tex_coords");
  obj.uniforms:set("tex_coords", u_tex_coords);

  u_model = gl.uniform.create("u_model");
  obj.uniforms:set("model", u_model);

  u_view = gl.uniform.create("u_view");
  obj.uniforms:set("view", u_view);

  u_proj = gl.uniform.create("u_projection");
  obj.uniforms:set("projection", u_proj);

  local u_tex_ratio = gl.uniform.create("u_tex_ratio");
  obj.uniforms:set("tex_ratio", u_tex_ratio);

  u_tex_ratio:set_vec2(obj.meta.animation:ratio());

  obj.data = {
    update_animation = update_animation,
    location = geom.vec3.new(0, 0, 0),
    current_action = "run_up",
    animation_frame = 0,
    uniforms = {
      tex_coords = u_tex_coords,
      model = u_model,
      view = u_view,
      proj = u_proj,
    },
  };
end

function update(obj)
  local player = obj.data;
  local animation = obj.meta.animation;

  if info.state ~= game.state.PLAYING then
    return;
  end

  local uv = animation:uv(player.current_action, player.animation_frame);
  if info.frames % 15 == 0 then
    player.animation_frame = animation:next(player.current_action, player.animation_frame);
  end
  player.uniforms.tex_coords:set_vec2(uv);

  local prev_loc = geom.vec3.new(player.location.x, player.location.y, player.location.z);

  if keyboard:check(input.key.W) == input.action.PRESS then
    player.location.y = player.location.y + 1;
  end

  if keyboard:check(input.key.A) == input.action.PRESS then
    player.location.x = player.location.x - 1;
  end

  if keyboard:check(input.key.S) == input.action.PRESS then
    player.location.y = player.location.y - 1;
  end

  if keyboard:check(input.key.D) == input.action.PRESS then
    player.location.x = player.location.x + 1;
  end

  if prev_loc.y > player.location.y then
    player:update_animation("run_down");
  elseif prev_loc.y < player.location.y then
    player:update_animation("run_up");
  elseif prev_loc.x > player.location.x then
    player:update_animation("run_left");
  elseif prev_loc.x < player.location.x then
    player:update_animation("run_right");
  elseif player.current_action ~= "still_left" and player.current_action ~= "still_right" and player.current_action ~= "still_up" and player.current_action ~= "still_down" then
    if player.current_action == "run_left" then
      player:update_animation("still_left")
    elseif player.current_action == "run_right" then
      player:update_animation("still_right")
    elseif player.current_action == "run_up" then
      player:update_animation("still_up")
    elseif player.current_action == "run_down" then
      player:update_animation("still_down")
    end
  end

  camera:move_to(player.location);

  local transform = geom.mat4.identity();
  transform = transform:translate(player.location);
  transform = transform:scale(geom.vec3.new(100, 100, 0));
  player.uniforms.model:set_mat4(transform);
  player.uniforms.view:set_mat4(camera:get_view());
  player.uniforms.proj:set_mat4(camera:get_projection());
end

function update_animation(player, anim)
  if player.current_action ~= anim then
    player.animation_frame = 0;
    player.current_action = anim;
  end
end
