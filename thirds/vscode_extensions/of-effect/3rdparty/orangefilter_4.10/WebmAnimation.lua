--* This Document is AutoGenerate by OrangeFilter, Don't Change it! *

---@meta

---
---[3.9]WebmAnimation[parent:]
---
---@class WebmAnimation
WebmAnimation = {}

---
---[3.9]constructor, 1 param
---
--- @param arg0 Context#context
--- @nodiscard
function WebmAnimation:new(arg0) end

---
---[3.9]load .webm
---
--- @param arg0 string
--- @return bool
--- @nodiscard
function WebmAnimation:load(arg0) end
---
---[3.9]frame width
---
--- @return OFUInt32
--- @nodiscard
function WebmAnimation:width() end
---
---[3.9]frame height
---
--- @return OFUInt32
--- @nodiscard
function WebmAnimation:height() end
---
---[3.9]duration
---
--- @return OFUInt32
--- @nodiscard
function WebmAnimation:duration() end
---
---[3.9]getFrame
---
--- @param arg0 int64
--- @param arg1 input texture
--- @return bool
--- @nodiscard
function WebmAnimation:getFrame(arg0, arg1) end
---
---[3.9]seek
---
--- @param arg0 int64
--- @nodiscard
function WebmAnimation:seek(arg0) end

return WebmAnimation
