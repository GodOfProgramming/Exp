function fps_text(self)
  self.text = string.format("fps = %d", game_info.fps);
end

function frame_count_text(self)
  self.text = string.format("frames = %d", game_info.frames);
end
