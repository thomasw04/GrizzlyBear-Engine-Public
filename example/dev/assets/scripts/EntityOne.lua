Entity = {}

function Entity:onSpawn()
  loadAnimation("anims/TestAnim.ganim")
end

function Entity:onUpdate(ts)
  
  if isKeyPressed(GRIZZLY_KEY_W) then
      self:setPosition(self:getPosition().x, self:getPosition().y + (ts * 0.02))
  elseif isKeyPressed(GRIZZLY_KEY_S) then
      self:setPosition(self:getPosition().x, self:getPosition().y - (ts * 0.02))
  end
	
  if isKeyPressed(GRIZZLY_KEY_A) then
      self:setPosition(self:getPosition().x - (ts * 0.02), self:getPosition().y)
  elseif isKeyPressed(GRIZZLY_KEY_D) then
      self:setPosition(self:getPosition().x + (ts * 0.02), self:getPosition().y)
  end

  if isKeyPressed(GRIZZLY_KEY_Q) then
      self:setRotation(self:getRotation() + (ts * 0.2))
  end

  if isKeyPressed(GRIZZLY_KEY_E) then
      self:setRotation(self:getRotation() - (ts * 0.2))
  end

  if isKeyPressed(GRIZZLY_KEY_P) then
    self:playAnimation("anims/TestAnim.ganim")
  end

  if isMousePressed(GRIZZLY_MOUSE_BUTTON_2) then
      LOG_INFO("Entity: ", self.__entityIndex__, ", Position: X: ", self:getPosition().x, " Y: ", self:getPosition().y)
  end
end

function Entity:onDelete()

end