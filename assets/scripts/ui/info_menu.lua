-- info_menu.lua
game = require('game');
info = game.info.instance();

function fps_text(self)
  self.text = string.format("fps = %d", info.fps);
end

function frame_count_text(self)
  self.text = string.format("frames = %d", info.frames);
end
