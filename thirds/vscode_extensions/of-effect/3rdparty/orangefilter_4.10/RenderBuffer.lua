--* This Document is AutoGenerate by OrangeFilter, Don't Change it! *

---@meta

---
---[3.3]render buffer[parent:BaseObject#baseobject]
---
---@class RenderBuffer
RenderBuffer = {}

---
---[3.3]set render buffer size and format,allocate memory
---
--- @param arg0 int
--- @param arg1 int
--- @param arg2 int
--- @return bool
--- @nodiscard
function RenderBuffer:set(arg0, arg1, arg2) end
---
---[3.3]get format
---
--- @return int
--- @nodiscard
function RenderBuffer:internalFormat() end
---
---[3.3]get width
---
--- @return int
--- @nodiscard
function RenderBuffer:width() end
---
---[3.3]get height
---
--- @return int
--- @nodiscard
function RenderBuffer:height() end
---
---[3.3]get gl render buffer id
---
--- @return int
--- @nodiscard
function RenderBuffer:glID() end

return RenderBuffer
