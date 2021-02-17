Index = 1;

Objects = GameObjects.instance();

Obj = nil;

Keys = Objects:keys();

for i,v in ipairs(Keys) do
  print(i, v);
end

function GetNextObject()
  local id = Keys[Index];

  if id == nil then
    Index = 1;
    return false;
  end

  Obj = Objects:get(id);

  if Obj == nil then
    Index = 1;
    return false;
  else
    Index = Index + 1;
    return true;
  end
end

function ObjectText(self)
  if Obj ~= nil then
    self.display_text = string.format("ID: %s", Obj.id);
  else
    self.display_text = "Nil object detected";
  end
end
