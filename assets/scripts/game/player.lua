inspect = require('inspect');
gl = require ('gl');
game = require('game');
info = game.info.instance();

self = nil;

current_action = "run";
animation_frame = 0;

function construct(player)
  self = player;
  self.uniforms:set("tex_coords", gl.uniform.new("u_tex_coords"));
  self.uniforms:set("tex_ratio", gl.uniform.new("u_tex_ratio"));
  local ratio = self.meta.animation:ratio();
  local tex_ratio = self.uniforms:get("tex_ratio");
  tex_ratio:set_vec2(ratio.x, ratio.y);
end

function update(_)
  local uv = self.meta.animation:uv(current_action, animation_frame);
  if info.frames % 15 == 0 then
    animation_frame = self.meta.animation:next(current_action, animation_frame);
  end
  local coords = self.uniforms:get("tex_coords");
  coords:set_vec2(uv.x, uv.y);
end