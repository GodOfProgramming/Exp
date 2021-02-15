function FpsText(self)
  self.display_text = string.format("fps = %d", GameInfo.fps);
end

function FrameCountText(self)
  self.display_text = string.format("frames = %d", GameInfo.frames);
end
