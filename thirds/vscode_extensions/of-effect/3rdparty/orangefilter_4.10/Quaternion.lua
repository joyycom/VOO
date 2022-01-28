--* This Document is AutoGenerate by OrangeFilter, Don't Change it! *

---@meta

---
---[3.2]Quaternion[parent:]
---
---@class Quaternion
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
---
---[3.2]w
---
---@field w float

Quaternion = {}

---
---[3.2]constructor, no param
---
--- @nodiscard
function Quaternion:new() end
---
---[3.2]constructor, 4 param
---
--- @param arg0 float
--- @param arg1 float
--- @param arg2 float
--- @param arg3 float
--- @nodiscard
function Quaternion:new(arg0, arg1, arg2, arg3) end

---
---[3.2]spherical linear interpolation to quaternion q by t
---
--- @param arg0 Quaternion#quaternion
--- @param arg1 float
--- @return Quaternion#quaternion
--- @nodiscard
function Quaternion:slerp(arg0, arg1) end
---
---[3.2]normalized linear quaternion interpolation to quaternion q by t
---
--- @param arg0 Quaternion#quaternion
--- @param arg1 float
--- @return Quaternion#quaternion
--- @nodiscard
function Quaternion:nlerp(arg0, arg1) end
---
---[3.2]inverted quaternion
---
--- @return Quaternion#quaternion
--- @nodiscard
function Quaternion:inverted() end
---
---[3.2]to euler angles
---
--- @return Vec3f#vec3f
--- @nodiscard
function Quaternion:toEulerAngles() end

---
---[3.2]quaternion, around axis rotate angle
---
--- @param arg0 float
--- @param arg1 Vec3f#vec3f
--- @return Quaternion#quaternion
--- @nodiscard
function Quaternion.AngleAxis(arg0, arg1) end
---
---[3.2]get a quaternion, rotate vector a to vector b
---
--- @param arg0 Vec3f#vec3f
--- @param arg1 Vec3f#vec3f
--- @return Quaternion#quaternion
--- @nodiscard
function Quaternion.FromToRotation(arg0, arg1) end
---
---[3.2]look to rotation
---
--- @param arg0 Vec3f#vec3f
--- @param arg1 Vec3f#vec3f
--- @return Quaternion#quaternion
--- @nodiscard
function Quaternion.LookRotation(arg0, arg1) end
---
---[3.2]euler angles to quaternion
---
--- @param arg0 float
--- @param arg1 float
--- @param arg2 float
--- @return Quaternion#quaternion
--- @nodiscard
function Quaternion.EulerDegree(arg0, arg1, arg2) end

return Quaternion
