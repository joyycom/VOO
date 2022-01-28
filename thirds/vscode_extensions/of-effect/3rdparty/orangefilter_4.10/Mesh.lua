--* This Document is AutoGenerate by OrangeFilter, Don't Change it! *

---@meta

---
---[3.7]mesh[parent:Object#object]
---
---@class Mesh
Mesh = {}

---
---[3.7]get vertex count
---
--- @return int
--- @nodiscard
function Mesh:getVertexCount() end
---
---[3.7]get vertices
---
--- @param arg0 FloatArray#floatarray
--- @param arg1 int
--- @nodiscard
function Mesh:getVertices(arg0, arg1) end
---
---[3.7]update vertices
---
--- @param arg0 FloatArray#floatarray
--- @param arg1 int
--- @nodiscard
function Mesh:updateVertices(arg0, arg1) end
---
---[3.7]get uvs
---
--- @param arg0 FloatArray#floatarray
--- @param arg1 int
--- @nodiscard
function Mesh:getUVs(arg0, arg1) end
---
---[3.7]update uvs
---
--- @param arg0 FloatArray#floatarray
--- @param arg1 int
--- @nodiscard
function Mesh:updateUVs(arg0, arg1) end
---
---[4.5]get submesh count
---
--- @return int
--- @nodiscard
function Mesh:getSubmeshCount() end
---
---[4.5]get index count in submesh
---
--- @param arg0 int
--- @return int
--- @nodiscard
function Mesh:getIndexCount(arg0) end
---
---[4.5]get indices in submesh
---
--- @param arg0 int
--- @param arg1 Uint16Array#uint16array
--- @param arg2 int
--- @nodiscard
function Mesh:getIndices(arg0, arg1, arg2) end
---
---[4.5]update indices in submesh, can add more indices or new submesh
---
--- @param arg0 int
--- @param arg1 Uint16Array#uint16array
--- @param arg2 int
--- @nodiscard
function Mesh:updateIndices(arg0, arg1, arg2) end
---
---[3.7]get blend shape count
---
--- @return int
--- @nodiscard
function Mesh:getBlendShapeCount() end
---
---[3.7]get blend shape name by index
---
--- @param arg0 int
--- @return string
--- @nodiscard
function Mesh:getBlendShapeName(arg0) end

return Mesh
