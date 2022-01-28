--* This Document is AutoGenerate by OrangeFilter, Don't Change it! *

---@meta

---
---[3.2]float matrix 4 x 4[parent:]
---
---@class Matrix4f
---
---[3.2]float values
---
---@field x float[]

Matrix4f = {}

---
---[3.2]set matrix values
---
--- @param arg0 float[]
--- @nodiscard
function Matrix4f:set(arg0) end
---
---[3.2]multiply Vec3f,apply a non-projection transform to point
---
--- @param arg0 Vec3f#vec3f
--- @return Vec3f#vec3f
--- @nodiscard
function Matrix4f:multiplyPoint3x4(arg0) end
---
---[3.2]multiply Vec3f,apply a non-projection transform to direction
---
--- @param arg0 Vec3f#vec3f
--- @return Vec3f#vec3f
--- @nodiscard
function Matrix4f:multiplyDirection(arg0) end
---
---[3.2]make matrix identity
---
--- @nodiscard
function Matrix4f:setIdentity() end
---
---[3.2]make all elements be 0
---
--- @nodiscard
function Matrix4f:setZero() end
---
---[3.2]apply a translation matrix
---
--- @param arg0 float
--- @param arg1 float
--- @param arg2 float
--- @nodiscard
function Matrix4f:translate(arg0, arg1, arg2) end
---
---[3.2]apply a scaling matrix
---
--- @param arg0 float
--- @param arg1 float
--- @param arg2 float
--- @nodiscard
function Matrix4f:scale(arg0, arg1, arg2) end
---
---[3.2]scale mix
---
--- @param arg0 float
--- @param arg1 float
--- @param arg2 float
--- @nodiscard
function Matrix4f:scaleMix(arg0, arg1, arg2) end
---
---[3.2]apply a rotation matrix
---
--- @param arg0 float
--- @param arg1 float
--- @param arg2 float
--- @nodiscard
function Matrix4f:rotate(arg0, arg1, arg2) end
---
---[3.2]get transposed matrix
---
--- @return Matrix4f#matrix4f
--- @nodiscard
function Matrix4f:transposed() end
---
---[3.2]get determinant of matrix
---
--- @return float
--- @nodiscard
function Matrix4f:determinant() end
---
---[3.2]get inverted matrix
---
--- @return Matrix4f#matrix4f
--- @nodiscard
function Matrix4f:inverted() end
---
---[3.2]extract translation, rotation, scale from matrix
---
--- @param arg0 Vec3f#vec3f
--- @param arg1 Vec3f#vec3f
--- @param arg2 Vec3f#vec3f
--- @nodiscard
function Matrix4f:decompose(arg0, arg1, arg2) end
---
---[3.2]set column
---
--- @param arg0 int
--- @param arg1 Vec4f#vec4f
--- @nodiscard
function Matrix4f:setCol(arg0, arg1) end
---
---[3.2]get column vector
---
--- @param arg0 int
--- @return Vec4f#vec4f
--- @nodiscard
function Matrix4f:getCol(arg0) end
---
---[3.2]get row vector
---
--- @param arg0 int
--- @return Vec4f#vec4f
--- @nodiscard
function Matrix4f:getRow(arg0) end
---
---[3.2]get translation vector
---
--- @return Vec3f#vec3f
--- @nodiscard
function Matrix4f:getTrans() end
---
---[3.2]get scale length
---
--- @return Vec3f#vec3f
--- @nodiscard
function Matrix4f:getScaleLen() end
---
---[3.2]get scale
---
--- @return Vec3f#vec3f
--- @nodiscard
function Matrix4f:getScale() end
---
---[3.2]get rotation quaternion
---
--- @return Quaternion#quaternion
--- @nodiscard
function Matrix4f:getQuat() end
---
---[3.2]get rotation vector
---
--- @return Vec3f#vec3f
--- @nodiscard
function Matrix4f:getRot() end

---
---[3.2]get reflect matrix to flip y
---
--- @return Matrix4f#matrix4f
--- @nodiscard
function Matrix4f.ReflectMat() end
---
---[3.2]build translation matrix
---
--- @param arg0 float
--- @param arg1 float
--- @param arg2 float
--- @return Matrix4f#matrix4f
--- @nodiscard
function Matrix4f.TransMat(arg0, arg1, arg2) end
---
---[3.2]build rotation matrix
---
--- @param arg0 float
--- @param arg1 float
--- @param arg2 float
--- @return Matrix4f#matrix4f
--- @nodiscard
function Matrix4f.RotMat(arg0, arg1, arg2) end
---
---[3.2]build scaling matrix
---
--- @param arg0 float
--- @param arg1 float
--- @param arg2 float
--- @return Matrix4f#matrix4f
--- @nodiscard
function Matrix4f.ScaleMat(arg0, arg1, arg2) end
---
---[3.2]build loot at matrix
---
--- @param arg0 Vec3f#vec3f
--- @param arg1 Vec3f#vec3f
--- @param arg2 Vec3f#vec3f
--- @return Matrix4f#matrix4f
--- @nodiscard
function Matrix4f.LookAtMat(arg0, arg1, arg2) end
---
---[3.2]build perspective matrix
---
--- @param arg0 float
--- @param arg1 float
--- @param arg2 float
--- @param arg3 float
--- @return Matrix4f#matrix4f
--- @nodiscard
function Matrix4f.PerspectiveMat(arg0, arg1, arg2, arg3) end
---
---[3.2]build orthographic matrix
---
--- @param arg0 float
--- @param arg1 float
--- @param arg2 float
--- @param arg3 float
--- @param arg4 float
--- @param arg5 float
--- @return Matrix4f#matrix4f
--- @nodiscard
function Matrix4f.OrthoMat(arg0, arg1, arg2, arg3, arg4, arg5) end
---
---[3.2]build transform matrix
---
--- @param arg0 Vec3f#vec3f
--- @param arg1 Quaternion#quaternion
--- @param arg2 Vec3f#vec3f
--- @return Matrix4f#matrix4f
--- @nodiscard
function Matrix4f.TRS(arg0, arg1, arg2) end

return Matrix4f
