function OnClose(self)
  print("closing window with title "..self.title);
  print(string.format("current fps is %d", GameInfo.fps));
end
