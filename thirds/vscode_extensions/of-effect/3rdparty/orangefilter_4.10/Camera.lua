--* This Document is AutoGenerate by OrangeFilter, Don't Change it! *

---@meta

---
---[3.2]camera[parent:]
---
---@class Camera
Camera = {}

---
---[3.2]set look at matrix
---
--- @param arg0 float
--- @param arg1 float
--- @param arg2 float
--- @param arg3 float
--- @param arg4 float
--- @param arg5 float
--- @param arg6 float
--- @param arg7 float
--- @param arg8 float
--- @nodiscard
function Camera:setLookAt(arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8) end
---
---[3.2]set view matrix
---
--- @param arg0 float[]
--- @nodiscard
function Camera:setViewMatrix(arg0) end
---
---[3.2]set projection matrix
---
--- @param arg0 float[]
--- @nodiscard
function Camera:setProjectionMatrix(arg0) end
---
---[3.2]convert point pos from projection space to world space
---
--- @param arg0 Vec2f#vec2f
--- @param arg1 float
--- @return Vec3f#vec3f
--- @nodiscard
function Camera:projToWorld(arg0, arg1) end
---
---[3.2]convert point pos from world space to projection space
---
--- @param arg0 Vec3f#vec3f
--- @return Vec3f#vec3f
--- @nodiscard
function Camera:worldToProj(arg0) end
---
---[3.2]get view matrix
---
--- @return Matrix4f#matrix4f
--- @nodiscard
function Camera:getViewMat() end
---
---[3.2]get projection matrix
---
--- @return Matrix4f#matrix4f
--- @nodiscard
function Camera:getProjMat() end

return Camera
