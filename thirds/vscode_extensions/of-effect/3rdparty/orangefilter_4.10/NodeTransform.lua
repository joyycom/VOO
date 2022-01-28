--* This Document is AutoGenerate by OrangeFilter, Don't Change it! *

---@meta

---
---[3.6]node transform[parent:]
---
---@class NodeTransform
---
---[3.6]renderer
---
---@field renderer RendererNode#renderernode
---
---[3.6]localPosition
---
---@field localPosition Vec3f#vec3f
---
---[3.6]localRotation
---
---@field localRotation Quaternion#quaternion
---
---[3.6]localScale
---
---@field localScale Vec3f#vec3f

NodeTransform = {}

---
---[3.6]find child by path
---
--- @param arg0 string
--- @return NodeTransform#nodetransform
--- @nodiscard
function NodeTransform:find(arg0) end
---
---[3.6]update node transform
---
--- @param arg0 Matrix4f#matrix4f
--- @nodiscard
function NodeTransform:updateTransform(arg0) end

return NodeTransform
