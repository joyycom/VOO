--* This Document is AutoGenerate by OrangeFilter, Don't Change it! *

---@meta

---
---[3.7]skinned mesh renderer[parent:Renderer#renderer]
---
---@class SkinnedMeshRenderer
SkinnedMeshRenderer = {}

---
---[3.7]get mesh
---
--- @return Mesh#mesh
--- @nodiscard
function SkinnedMeshRenderer:getMesh() end
---
---[3.7]get blend shape weight
---
--- @param arg0 int
--- @return float
--- @nodiscard
function SkinnedMeshRenderer:getBlendShapeWeight(arg0) end
---
---[3.7]set blend shape weight
---
--- @param arg0 int
--- @param arg1 float
--- @nodiscard
function SkinnedMeshRenderer:setBlendShapeWeight(arg0, arg1) end
---
---[4.6]get vertices
---
--- @param arg0 FloatArray#floatarray
--- @param arg1 int
--- @nodiscard
function SkinnedMeshRenderer:getBlendShapeVertices(arg0, arg1) end
---
---[4.6]enable gpu blend shape
---
--- @nodiscard
function SkinnedMeshRenderer:tryEnableGPUBlendShape() end
---
---[4.6]get if gpu blend shape enabled or not
---
--- @return bool
--- @nodiscard
function SkinnedMeshRenderer:isGPUBlendShapeEnabled() end
---
---[4.1]replace bones
---
--- @param arg0 Transform#transform
--- @nodiscard
function SkinnedMeshRenderer:replaceBones(arg0) end

return SkinnedMeshRenderer
