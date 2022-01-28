--* This Document is AutoGenerate by OrangeFilter, Don't Change it! *

---@meta

---
---[3.2]line 2d render[parent:Render]
---
---@class Line2DRender
Line2DRender = {}

---
---[3.2]constructor, 2 param
---
--- @param arg0 FloatArray#floatarray
--- @param arg1 int
--- @nodiscard
function Line2DRender:new(arg0, arg1) end

---
---[3.2]get vertex count
---
--- @return int
--- @nodiscard
function Line2DRender:getVertexCount() end
---
---[3.2]update vertex positions
---
--- @param arg0 FloatArray#floatarray
--- @param arg1 int
--- @nodiscard
function Line2DRender:updatePoints(arg0, arg1) end
---
---[3.2]update vertex colors
---
--- @param arg0 FloatArray#floatarray
--- @param arg1 int
--- @nodiscard
function Line2DRender:updateColors(arg0, arg1) end
---
---[3.2]draw this render
---
--- @param arg0 Program#program
--- @param arg1 bool
--- @nodiscard
function Line2DRender:draw(arg0, arg1) end

return Line2DRender
