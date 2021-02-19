function FpsText(self)
  self.text = string.format("fps = %d", GameInfo.fps);
end

function FrameCountText(self)
  self.text = string.format("frames = %d", GameInfo.frames);
end
