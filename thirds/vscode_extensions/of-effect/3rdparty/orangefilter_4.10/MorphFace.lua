--* This Document is AutoGenerate by OrangeFilter, Don't Change it! *

---@meta

---
---[4.2]MorphFace[parent:]
---
---@class MorphFace
MorphFace = {}

---
---[4.2]constructor, 1 param
---
--- @param arg0 Context#context
--- @nodiscard
function MorphFace:new(arg0) end

---
---[4.2]init
---
--- @param arg0 FloatArray#floatarray
--- @param arg1 FloatArray#floatarray
--- @param arg2 int
--- @param arg3 int
--- @param arg4 int
--- @return bool
--- @nodiscard
function MorphFace:init(arg0, arg1, arg2, arg3, arg4) end
---
---[4.2]set duration
---
--- @param arg0 float
--- @nodiscard
function MorphFace:setDuration(arg0) end
---
---[4.2]set subdivision num
---
--- @param arg0 int
--- @param arg1 int
--- @nodiscard
function MorphFace:setSubdivisionNum(arg0, arg1) end
---
---[4.2]update buffer
---
--- @param arg0 FloatArray#floatarray
--- @param arg1 FloatArray#floatarray
--- @param arg2 float
--- @nodiscard
function MorphFace:updateVertices(arg0, arg1, arg2) end
---
---[4.2]update buffer
---
--- @param arg0 FloatArray#floatarray
--- @nodiscard
function MorphFace:updateIndices(arg0) end

return MorphFace
