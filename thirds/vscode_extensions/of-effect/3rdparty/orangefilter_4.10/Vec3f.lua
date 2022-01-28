--* This Document is AutoGenerate by OrangeFilter, Don't Change it! *

---@meta

---
---[3.2]float vector has 3 components[parent:]
---
---@class Vec3f
---
---[3.2]x
---
---@field x float
---
---[3.2]y
---
---@field y float
---
---[3.2]z
---
---@field z float

Vec3f = {}

---
---[3.2]constructor, no param
---
--- @nodiscard
function Vec3f:new() end
---
---[3.2]constructor, 3 param
---
--- @param arg0 float
--- @param arg1 float
--- @param arg2 float
--- @nodiscard
function Vec3f:new(arg0, arg1, arg2) end

---
---[3.2]set value
---
--- @param arg0 float
--- @param arg1 float
--- @param arg2 float
--- @nodiscard
function Vec3f:set(arg0, arg1, arg2) end
---
---[3.2]get vector length
---
--- @return float
--- @nodiscard
function Vec3f:length() end
---
---[3.2]dot with a vector
---
--- @param arg0 Vec3f#vec3f
--- @return float
--- @nodiscard
function Vec3f:dot(arg0) end
---
---[3.2]cross with a vector
---
--- @param arg0 Vec3f#vec3f
--- @return Vec3f#vec3f
--- @nodiscard
function Vec3f:cross(arg0) end
---
---[3.2]sqr of length,fast than length
---
--- @return float
--- @nodiscard
function Vec3f:sqrLength() end
---
---[3.2]get a normalized vector
---
--- @return Vec3f#vec3f
--- @nodiscard
function Vec3f:normalized() end
---
---[3.2]normalize this vector
---
--- @nodiscard
function Vec3f:normalize() end
---
---[3.2]dir to a rotation vector
---
--- @return Vec3f#vec3f
--- @nodiscard
function Vec3f:toRotation() end
---
---[3.2]lerp to a vector by t
---
--- @param arg0 [Vec3f#vec3f)
--- @param arg1 float
--- @return [Vec3f#vec3f)
--- @nodiscard
function Vec3f:lerp(arg0, arg1) end

---
---[3.2]lerp a to b by t
---
--- @param arg0 Vec3f#vec3f
--- @param arg1 Vec3f#vec3f
--- @param arg2 float
--- @return Vec3f#vec3f
--- @nodiscard
function Vec3f.Lerp(arg0, arg1, arg2) end
---
---[3.2]normalize a vector
---
--- @param arg0 Vec3f#vec3f
--- @return Vec3f#vec3f
--- @nodiscard
function Vec3f.Normalize(arg0) end
---
---[3.2]get angle in degree between 2 vector
---
--- @param arg0 Vec3f#vec3f
--- @param arg1 Vec3f#vec3f
--- @return float
--- @nodiscard
function Vec3f.Angle(arg0, arg1) end

return Vec3f
