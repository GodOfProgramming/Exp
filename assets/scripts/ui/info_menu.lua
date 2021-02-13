function on_close(self)
  print("closing window with title "..self.title);
  local fmt = string.format("current fps is %d", game_info.fps);
  print(fmt);
end
