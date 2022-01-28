--* This Document is AutoGenerate by OrangeFilter, Don't Change it! *

---@meta

---
---[3.3]color curves[parent:]
---
---@class Gradient
Gradient = {}

---
---[3.3]constructor, no param
---
--- @nodiscard
function Gradient:new() end

---
---[3.3]add color key
---
--- @param arg0 float
--- @param arg1 Color#color
--- @nodiscard
function Gradient:addKeyColor(arg0, arg1) end
---
---[3.3]add alpha key
---
--- @param arg0 float
--- @param arg1 float
--- @nodiscard
function Gradient:addKeyAlpha(arg0, arg1) end
---
---[3.6]evaluate color by time
---
--- @param arg0 float
--- @return Color#color
--- @nodiscard
function Gradient:evaluate(arg0) end

return Gradient
