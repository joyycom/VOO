--* This Document is AutoGenerate by OrangeFilter, Don't Change it! *

---@meta

---
---[4.5]svga path data[parent:]
---
---@class SvgaPathData
SvgaPathData = {}

---
---[4.5]constructor, no param
---
--- @nodiscard
function SvgaPathData:new() end

---
---[4.5]subdivide step
---
--- @param arg0 int
--- @nodiscard
function SvgaPathData:updateMaxSubdivideStep(arg0) end
---
---[4.5]subdivide step
---
--- @param arg0 int
--- @nodiscard
function SvgaPathData:updateOvalSubdivideStep(arg0) end
---
---[4.5]add command
---
--- @param arg0 int
--- @param arg1 FloatArray#floatarray
--- @nodiscard
function SvgaPathData:addCommand(arg0, arg1) end
---
---[4.5]generate triangles
---
--- @nodiscard
function SvgaPathData:generateTriangles() end
---
---[4.5]generate triangles
---
--- @param arg0 float
--- @param arg1 bool
--- @nodiscard
function SvgaPathData:generateStrokeTriangles(arg0, arg1) end
---
---[4.5]get triangle soup count
---
--- @return int
--- @nodiscard
function SvgaPathData:getTriangleSoupCount() end
---
---[4.5]get triangle soup count
---
--- @param arg0 int
--- @return TriangleSoup
--- @nodiscard
function SvgaPathData:getTriangleSoup(arg0) end
---
---[4.5]clear
---
--- @nodiscard
function SvgaPathData:clear() end

return SvgaPathData
