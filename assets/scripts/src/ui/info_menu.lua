-- info_menu.lua
game = require('game');
info = game.info.instance();
world = game.world.instance();

function fps_text(self)
  self.text = string.format("fps = %d", info.fps);
end

function frame_count_text(self)
  self.text = string.format("frames = %d", info.frames);
end

function player_location(self)
  local player = world:lookup(info.player_id);
  self.text = string.format("Player location = (%.2f, %.2f, %.2f)", player.pos.x, player.pos.y, player.pos.z);
end

function world_count(self)
  self.text = string.format("Objects in world: %d", world:count());
end
