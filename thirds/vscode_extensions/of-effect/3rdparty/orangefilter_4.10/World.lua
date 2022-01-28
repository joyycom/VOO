--* This Document is AutoGenerate by OrangeFilter, Don't Change it! *

---@meta

---
---[3.6]world, manage all entities[parent:]
---
---@class World
World = {}

---
---[3.6]update world
---
--- @nodiscard
function World:update() end
---
---[4.1]update world with time
---
--- @param arg0 float
--- @nodiscard
function World:updateWithTime(arg0) end
---
---[3.6]render world
---
--- @nodiscard
function World:render() end

---
---[3.6]create world, only 1 world instance can be create
---
--- @param arg0 Context
--- @return World
--- @nodiscard
function World.Create(arg0) end
---
---[3.6]destroy world
---
--- @param arg0 World
--- @nodiscard
function World.Destroy(arg0) end
---
---[4.1]bind current world
---
--- @param arg0 World
--- @nodiscard
function World.Bind(arg0) end

return World
