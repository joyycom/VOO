--* This Document is AutoGenerate by OrangeFilter, Don't Change it! *

---@meta

---
---[3.3]gl vertex buffer or index buffer,can create by context[parent:BaseObject#baseobject]
---
---@class Buffer
Buffer = {}

---
---[3.4]update buffer data
---
--- @param arg0 int
--- @param arg1 data
--- @param arg2 data index offset
--- @param arg3 data element count
--- @nodiscard
function Buffer:updateFloatArrayOffset(arg0, arg1, arg2, arg3) end
---
---[3.4]update buffer data
---
--- @param arg0 int
--- @param arg1 data
--- @param arg2 data index offset
--- @param arg3 data element count
--- @nodiscard
function Buffer:updateUint16ArrayOffset(arg0, arg1, arg2, arg3) end
---
---[3.3]get buffer type
---
--- @return GL_BUFFER_TYPE#gl_buffer_type
--- @nodiscard
function Buffer:type() end
---
---[3.3]get buffer size in bytes
---
--- @return int
--- @nodiscard
function Buffer:size() end
---
---[3.3]get gl buffer id
---
--- @return int
--- @nodiscard
function Buffer:glID() end
---
---[3.3]bind buffer
---
--- @nodiscard
function Buffer:bind() end
---
---[3.3]unbind buffer
---
--- @nodiscard
function Buffer:unbind() end

return Buffer
