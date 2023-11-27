print("Hello World")

Entity = {}

function Entity:onSpawn()

end

function Entity:onUpdate(ts)

  self:setPosition(self:getPosition().x + (ts * 0.002), 0)
  self:setRotation(self:getRotation() - (ts * 0.5))
  self:setSize(self:getSize().x + (ts * 0.001), self:getSize().y + (ts * 0.001))
end

function Entity:onDelete()

end