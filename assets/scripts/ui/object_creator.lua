Index = 1;

Objects = GameObjects.instance();

Obj = nil;
SelectedObj = nil;

Keys = Objects:keys();

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
    self.text = string.format("ID: %s", Obj.id);
  else
    self.text = "Nil object detected";
  end

  self.btn_text = "Set";
end

function SetObject(self)
  SelectedObj = Objects:get(self.text);
end

function ObjHasValue()
  return SelectedObj ~= nil;
end

function GetWireframeValue(self)
  self.text = "Wireframe: "..tostring(SelectedObj.draw_desc.wireframe);
end
