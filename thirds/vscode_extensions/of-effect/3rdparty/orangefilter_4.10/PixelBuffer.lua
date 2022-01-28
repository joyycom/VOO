--* This Document is AutoGenerate by OrangeFilter, Don't Change it! *

---@meta

---
---[4.4]PixelBuffer[parent:]
---
---@class PixelBuffer
PixelBuffer = {}

---
---[4.4]constructor, 3 param
---
--- @param arg0 Context#context
--- @param arg1 int
--- @param arg2 int
--- @nodiscard
function PixelBuffer:new(arg0, arg1, arg2) end
---
---[4.5]constructor, 4 param
---
--- @param arg0 Context#context
--- @param arg1 int
--- @param arg2 int
--- @param arg3 int
--- @nodiscard
function PixelBuffer:new(arg0, arg1, arg2, arg3) end

---
---[4.4]copy tex to memory
---
--- @param arg0 OF_Texture#of_texture
--- @param arg1 ByteArray#bytearray
--- @nodiscard
function PixelBuffer:copyToMemory(arg0, arg1) end

return PixelBuffer
