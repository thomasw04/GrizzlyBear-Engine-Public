Scene = {}

function Scene:onLoad()

    Entity entity = self:spawn_entity({1.0f, 2.0f, 3.0f})

    entity:add_component("Mesh2D", {1.0f, 1.0f, 1.0f, 1.0f})
    entity:add_component("Script", "assets/scripts/EntityOne.lua")

    Entity entity1 = self:build_entity({1.0f, 1.0f, 1.0f})

    entity:add_component("Mesh2D", {1.0f, 1.0f, 1.0f, 1.0f})
    entity:add_component("Script", "assets/scripts/EntityOne.lua")

    self:spawn_entity(entity)
end

function Scene:onUpdate(ts)

    
end

function Scene:onShutdown()

end