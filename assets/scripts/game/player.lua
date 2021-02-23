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

function construct(player)
  self = player;

  u_tex_coords = gl.uniform.create("u_tex_coords");
  self.uniforms:set("tex_coords", u_tex_coords);

  local u_tex_ratio = gl.uniform.create("u_tex_ratio");
  self.uniforms:set("tex_ratio", u_tex_ratio);

  u_color = gl.uniform.create("u_color");
  self.uniforms:set("color", u_color);

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

  u_color:set_vec3(color);
end