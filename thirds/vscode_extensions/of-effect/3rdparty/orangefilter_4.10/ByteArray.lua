--* This Document is AutoGenerate by OrangeFilter, Don't Change it! *

---@meta

---
---[3.6]ByteArray[parent:]
---
---@class ByteArray
ByteArray = {}

---
---[3.6]constructor, 1 param
---
--- @param arg0 int
--- @nodiscard
function ByteArray:new(arg0) end

---
---[3.6]realloc array data
---
--- @param arg0 int
--- @nodiscard
function ByteArray:realloc(arg0) end
---
---[3.6]get element value
---
--- @param arg0 int
--- @return int
--- @nodiscard
function ByteArray:get(arg0) end
---
---[3.6]set element value
---
--- @param arg0 int
--- @param arg1 int
--- @nodiscard
function ByteArray:set(arg0, arg1) end
---
---[3.6]get array size
---
--- @return int
--- @nodiscard
function ByteArray:size() end

---
---[4.5]get byte size of Byte
---
--- @return int
--- @nodiscard
function ByteArray.byteSizeOfByte() end

return ByteArray
