inspect = require('inspect');
gl = require ('gl');
geom = require('geom');
game = require('game');
info = game.info.instance();

self = nil;

current_action = "run_up";
animation_frame = 0;

u_tex_coords = nil;
u_color = nil;
u_transform = nil;

function construct(player)
  self = player;

  u_tex_coords = gl.uniform.create("u_tex_coords");
  self.uniforms:set("tex_coords", u_tex_coords);

  local u_tex_ratio = gl.uniform.create("u_tex_ratio");
  self.uniforms:set("tex_ratio", u_tex_ratio);

  u_color = gl.uniform.create("u_color");
  self.uniforms:set("color", u_color);

  u_transform = gl.uniform.create("u_transform");
  self.uniforms:set("transform", u_transform);

  u_tex_ratio:set_vec2(self.meta.animation:ratio());
end

function update(_)
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

  local scale = 0.1;
  local color = geom.vec3.new();
  color.x = math.sin(scale * info.frames) / 2 + 0.5;
  color.y = math.cos(scale * info.frames) / 2 + 0.5;
  color.z = color.x * color.y;

  local foo = geom.vec3.new(1, 1, 1);
  color = color:mul(foo);

  u_color:set_vec3(color);

  local transform = geom.mat4.identity();
  transform = transform:translate(geom.vec3.new(0.5, -0.5, 0.0));
  transform = transform:rotate(math.rad(info.frames), geom.vec3.new(0, 0, 1));
  transform = transform:scale(geom.vec3.new(0.5, 0.5, 0.5));
  u_transform:set_mat4(transform);
end