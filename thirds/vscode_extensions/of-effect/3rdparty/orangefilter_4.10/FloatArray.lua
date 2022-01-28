--* This Document is AutoGenerate by OrangeFilter, Don't Change it! *

---@meta

---
---[3.2]FloatArray[parent:]
---
---@class FloatArray
FloatArray = {}

---
---[3.2]constructor, 1 param
---
--- @param arg0 int
--- @nodiscard
function FloatArray:new(arg0) end

---
---[3.4]realloc array data
---
--- @param arg0 int
--- @nodiscard
function FloatArray:realloc(arg0) end
---
---[3.2]get element value
---
--- @param arg0 int
--- @return float
--- @nodiscard
function FloatArray:get(arg0) end
---
---[3.2]set element value
---
--- @param arg0 int
--- @param arg1 float
--- @nodiscard
function FloatArray:set(arg0, arg1) end
---
---[3.2]get array size
---
--- @return int
--- @nodiscard
function FloatArray:size() end

---
---[4.5]get byte size of float
---
--- @return int
--- @nodiscard
function FloatArray.byteSizeOfFloat() end

return FloatArray
