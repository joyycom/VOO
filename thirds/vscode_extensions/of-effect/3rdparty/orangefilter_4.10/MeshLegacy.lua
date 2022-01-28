--* This Document is AutoGenerate by OrangeFilter, Don't Change it! *

---@meta

---
---[3.6]mesh[parent:]
---
---@class MeshLegacy
MeshLegacy = {}

---
---[3.6]get mesh count
---
--- @return int
--- @nodiscard
function MeshLegacy:getMeshCount() end
---
---[3.7]update mesh data
---
--- @param arg0 int
--- @param arg1 FloatArray#floatarray
--- @param arg2 int
--- @param arg3 Uint16Array#uint16array
--- @param arg4 int
--- @nodiscard
function MeshLegacy:updateMesh(arg0, arg1, arg2, arg3, arg4) end
---
---[3.7]get submesh count
---
--- @param arg0 int
--- @return int
--- @nodiscard
function MeshLegacy:getSubmeshCount(arg0) end
---
---[3.7]get submesh index count
---
--- @param arg0 int
--- @param arg1 int
--- @return int
--- @nodiscard
function MeshLegacy:getIndexCount(arg0, arg1) end
---
---[3.7]get submesh indices
---
--- @param arg0 int
--- @param arg1 int
--- @param arg2 Uint16Array#uint16array
--- @nodiscard
function MeshLegacy:getIndices(arg0, arg1, arg2) end
---
---[3.6]set update mesh vertices scale
---
--- @param arg0 Vec3f#vec3f
--- @nodiscard
function MeshLegacy:setUpdateVertexScale(arg0) end
---
---[3.6]update mesh vertices
---
--- @param arg0 int
--- @param arg1 float[]
--- @param arg2 int
--- @nodiscard
function MeshLegacy:updateVertices(arg0, arg1, arg2) end
---
---[3.6]update mesh texture coords
---
--- @param arg0 int
--- @param arg1 float[]
--- @param arg2 int
--- @nodiscard
function MeshLegacy:updateTextureCoords(arg0, arg1, arg2) end
---
---[3.6]update mesh vertex buffer
---
--- @param arg0 int
--- @nodiscard
function MeshLegacy:updateVertexBuffer(arg0) end
---
---[3.7]get mesh dynamic vertex position
---
--- @param arg0 int
--- @param arg1 int
--- @return Vec3f#vec3f
--- @nodiscard
function MeshLegacy:getDynamicVertex(arg0, arg1) end
---
---[3.7]enable draw wireframe or not
---
--- @param arg0 bool
--- @nodiscard
function MeshLegacy:enableWireframeMode(arg0) end

return MeshLegacy
