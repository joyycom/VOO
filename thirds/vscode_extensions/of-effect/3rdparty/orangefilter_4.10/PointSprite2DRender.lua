--* This Document is AutoGenerate by OrangeFilter, Don't Change it! *

---@meta

---
---[3.2]point sprite 2d render[parent:Render#render]
---
---@class PointSprite2DRender
PointSprite2DRender = {}

---
---[3.2]constructor, 2 param
---
--- @param arg0 FloatArray#floatarray
--- @param arg1 int
--- @nodiscard
function PointSprite2DRender:new(arg0, arg1) end
---
---[3.2]constructor, 3 param
---
--- @param arg0 FloatArray#floatarray
--- @param arg1 int
--- @param arg2 int
--- @nodiscard
function PointSprite2DRender:new(arg0, arg1, arg2) end

---
---[3.2]draw this render
---
--- @param arg0 Program#program
--- @param arg1 bool
--- @nodiscard
function PointSprite2DRender:draw(arg0, arg1) end
---
---[3.2]draw part of points
---
--- @param arg0 Program#program
--- @param arg1 int
--- @nodiscard
function PointSprite2DRender:drawCount(arg0, arg1) end
---
---[3.2]update vertex positions
---
--- @param arg0 FloatArray#floatarray
--- @param arg1 int
--- @nodiscard
function PointSprite2DRender:updatePoints(arg0, arg1) end
---
---[3.2]update vertex colors
---
--- @param arg0 FloatArray#floatarray
--- @param arg1 int
--- @nodiscard
function PointSprite2DRender:updateColors(arg0, arg1) end
---
---[3.2]get vertex count
---
--- @return int
--- @nodiscard
function PointSprite2DRender:getVerNum() end

return PointSprite2DRender
