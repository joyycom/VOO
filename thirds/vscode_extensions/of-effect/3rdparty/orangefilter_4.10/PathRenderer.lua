--* This Document is AutoGenerate by OrangeFilter, Don't Change it! *

---@meta

---
---[3.2]path renderer[parent:]
---
---@class PathRenderer
PathRenderer = {}

---
---[3.2]constructor, 1 param
---
--- @param arg0 Context#context
--- @nodiscard
function PathRenderer:new(arg0) end

---
---[3.2]set point lifetime
---
--- @param arg0 float
--- @nodiscard
function PathRenderer:setPointLifeTime(arg0) end
---
---[3.2]set insert point min distance
---
--- @param arg0 float
--- @nodiscard
function PathRenderer:setInsertPointsMinDistance(arg0) end
---
---[3.2]set new point min distance
---
--- @param arg0 float
--- @nodiscard
function PathRenderer:setNewPointMinDistance(arg0) end
---
---[3.2]set continue new point max time
---
--- @param arg0 float
--- @nodiscard
function PathRenderer:setContinueNewPointMaxTime(arg0) end
---
---[3.2]add point
---
--- @param arg0 Vec3f#vec3f
--- @param arg1 float
--- @return Vec3f#vec3f)[]
--- @nodiscard
function PathRenderer:addPoint(arg0, arg1) end
---
---[3.2]clear all points
---
--- @nodiscard
function PathRenderer:clearPoints() end
---
---[3.2]update renderer
---
--- @param arg0 float
--- @return int
--- @nodiscard
function PathRenderer:update(arg0) end
---
---[3.2]get points
---
--- @return Vec3f#vec3f)[]
--- @nodiscard
function PathRenderer:getPoints() end

return PathRenderer
