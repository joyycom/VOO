--* This Document is AutoGenerate by OrangeFilter, Don't Change it! *

---@meta

---
---[3.2]mesh 2d render[parent:Render#render]
---
---@class Mesh2dRender
Mesh2dRender = {}

---
---[3.2]constructor, 5 param
---
--- @param arg0 FloatArray#floatarray
--- @param arg1 FloatArray#floatarray
--- @param arg2 int
--- @param arg3 IntArray
--- @param arg4 int
--- @nodiscard
function Mesh2dRender:new(arg0, arg1, arg2, arg3, arg4) end
---
---[4.7]constructor, 7 param
---
--- @param arg0 FloatArray#floatarray
--- @param arg1 FloatArray#floatarray
--- @param arg2 int
--- @param arg3 Uint16Array
--- @param arg4 int
--- @param arg5 bool
--- @nodiscard
function Mesh2dRender:new(arg0, arg1, arg2, arg3, arg4, arg5) end

---
---[3.2]draw this render
---
--- @param arg0 Program#program
--- @param arg1 bool
--- @nodiscard
function Mesh2dRender:draw(arg0, arg1) end
---
---[3.2]update front range or all positions
---
--- @param arg0 float[]
--- @param arg1 int
--- @nodiscard
function Mesh2dRender:updateSubPositions(arg0, arg1) end
---
---[3.2]update front range or all texture coords
---
--- @param arg0 float[]
--- @param arg1 int
--- @nodiscard
function Mesh2dRender:updateSubTexureCoords(arg0, arg1) end
---
---[3.2]update front range or all colors
---
--- @param arg0 float[]
--- @param arg1 int
--- @nodiscard
function Mesh2dRender:updateSubColors(arg0, arg1) end
---
---[4.4]update triangle indices
---
--- @param arg0 IntArray
--- @param arg1 int
--- @nodiscard
function Mesh2dRender:updateTriIndices(arg0, arg1) end
---
---[4.7]update triangle indices
---
--- @param arg0 Uint16Array
--- @param arg1 int
--- @nodiscard
function Mesh2dRender:updateUint16TriIndices(arg0, arg1) end
---
---[3.2]set draw in wireframe
---
--- @param arg0 bool
--- @param arg1 bool
--- @nodiscard
function Mesh2dRender:setWireframe(arg0, arg1) end
---
---[3.2]get vertex count
---
--- @return int
--- @nodiscard
function Mesh2dRender:getVerNum() end
---
---[3.2]get triangle count
---
--- @return int
--- @nodiscard
function Mesh2dRender:getTriNum() end

return Mesh2dRender
