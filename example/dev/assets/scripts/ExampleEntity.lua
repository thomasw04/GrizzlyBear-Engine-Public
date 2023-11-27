Entity = {}

function Entity:onSpawn()
  loadAnimation("anims/TestAnim.ganim")
end

function Entity:onUpdate(ts)
  if isKeyPressed(GRIZZLY_KEY_Q) then
      self:setRotation(self:getRotation() + (ts * 0.2))
  end

  if isKeyPressed(GRIZZLY_KEY_E) then
      self:setRotation(self:getRotation() - (ts * 0.2))
  end

  if isKeyPressed(GRIZZLY_KEY_P) then
    self:playAnimation("anims/TestAnim.ganim")
  end
end

function Entity:onDelete()

end



