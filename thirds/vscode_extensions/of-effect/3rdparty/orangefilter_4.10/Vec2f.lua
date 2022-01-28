--* This Document is AutoGenerate by OrangeFilter, Don't Change it! *

---@meta

---
---[3.2]float vector has 2 components[parent:]
---
---@class Vec2f
---
---[3.2]x
---
---@field x float
---
---[3.2]y
---
---@field y float

Vec2f = {}

---
---[3.2]constructor, no param
---
--- @nodiscard
function Vec2f:new() end
---
---[3.2]constructor, 2 param
---
--- @param arg0 float
--- @param arg1 float
--- @nodiscard
function Vec2f:new(arg0, arg1) end

---
---[4.0]get vector length
---
--- @return float
--- @nodiscard
function Vec2f:length() end
---
---[4.3]sqr of length, fast than length
---
--- @return float
--- @nodiscard
function Vec2f:sqrLength() end
---
---[4.3]dot with a vector
---
--- @param arg0 Vec2f#vec2f
--- @return float
--- @nodiscard
function Vec2f:dot(arg0) end
---
---[4.3]cross with a vector
---
--- @param arg0 Vec2f#vec2f
--- @return float
--- @nodiscard
function Vec2f:cross(arg0) end

---
---[4.3]get angle in degree between 2 vector
---
--- @param arg0 Vec2f#vec2f
--- @param arg1 Vec2f#vec2f
--- @return float
--- @nodiscard
function Vec2f.Angle(arg0, arg1) end

return Vec2f
