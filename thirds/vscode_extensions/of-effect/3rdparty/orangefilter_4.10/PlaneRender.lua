--* This Document is AutoGenerate by OrangeFilter, Don't Change it! *

---@meta

---
---[3.2]PlaneRender[parent:Render#render]
---
---@class PlaneRender
PlaneRender = {}

---
---[3.2]constructor, 5 param
---
--- @param arg0 float
--- @param arg1 float
--- @param arg2 float
--- @param arg3 float
--- @param arg4 bool
--- @nodiscard
function PlaneRender:new(arg0, arg1, arg2, arg3, arg4) end

---
---[3.2]draw this render
---
--- @param arg0 Program#program
--- @param arg1 bool
--- @nodiscard
function PlaneRender:draw(arg0, arg1) end
---
---[4.5]draw instance this render
---
--- @param arg0 Program#program
--- @param arg1 int
--- @nodiscard
function PlaneRender:drawInstanced(arg0, arg1) end

return PlaneRender
