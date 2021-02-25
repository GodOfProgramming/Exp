-- player.lua
gl = require ('gl');
geom = require('geom');
input = require('input');
game = require('game');
info = game.info.instance();
camera = game.camera.instance();
world = game.world.instance();
keyboard = input.keyboard.instance();

function construct(player)
  u_tex_coords = gl.uniform.create("u_tex_coords");
  player.uniforms:set("tex_coords", u_tex_coords);

  u_model = gl.uniform.create("u_model");
  player.uniforms:set("model", u_model);

  u_view = gl.uniform.create("u_view");
  player.uniforms:set("view", u_view);

  u_proj = gl.uniform.create("u_projection");
  player.uniforms:set("projection", u_proj);

  local u_tex_ratio = gl.uniform.create("u_tex_ratio");
  player.uniforms:set("tex_ratio", u_tex_ratio);

  u_tex_ratio:set_vec2(player.meta.animation:ratio());

  player.data = {
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

function update(player)
  if info.state ~= game.state.PLAYING then
    return;
  end

  local player_data = player.data;
  local animation = player.meta.animation;

  local prev_pos = geom.vec3.new(player.pos.x, player.pos.y, player.pos.z);

  if keyboard:check(input.key.W) == input.action.PRESS then
    player.pos.y = player.pos.y + 1;
  end

  if keyboard:check(input.key.A) == input.action.PRESS then
    player.pos.x = player.pos.x - 1;
  end

  if keyboard:check(input.key.S) == input.action.PRESS then
    player.pos.y = player.pos.y - 1;
  end

  if keyboard:check(input.key.D) == input.action.PRESS then
    player.pos.x = player.pos.x + 1;
  end

  if keyboard:check(input.key.C) == input.action.PRESS and keyboard:changed(input.key.C) then
    world:spawn("exp.test.random.square", player.pos);
  end

  if prev_pos.y > player.pos.y then
    player_data:update_animation("run_down");
  elseif prev_pos.y < player.pos.y then
    player_data:update_animation("run_up");
  elseif prev_pos.x > player.pos.x then
    player_data:update_animation("run_left");
  elseif prev_pos.x < player.pos.x then
    player_data:update_animation("run_right");
  elseif player_data.current_action ~= "still_left" and player_data.current_action ~= "still_right" and player_data.current_action ~= "still_up" and player_data.current_action ~= "still_down" then
    if player_data.current_action == "run_left" then
      player_data:update_animation("still_left")
    elseif player_data.current_action == "run_right" then
      player_data:update_animation("still_right")
    elseif player_data.current_action == "run_up" then
      player_data:update_animation("still_up")
    elseif player_data.current_action == "run_down" then
      player_data:update_animation("still_down")
    end
  end

  camera:move_to(player.pos);

  local uv = animation:uv(player_data.current_action, player_data.animation_frame);
  if info.frames % 15 == 0 then
    player_data.animation_frame = animation:next(player_data.current_action, player_data.animation_frame);
  end
  player_data.uniforms.tex_coords:set_vec2(uv);

  local transform = geom.mat4.identity();
  transform = transform:translate(player.pos);
  transform = transform:scale(geom.vec3.new(100, 100, 0));
  player_data.uniforms.model:set_mat4(transform);
  player_data.uniforms.view:set_mat4(camera:get_view());
  player_data.uniforms.proj:set_mat4(camera:get_projection());
end

function update_animation(player, anim)
  if player.current_action ~= anim then
    player.animation_frame = 0;
    player.current_action = anim;
  end
end
