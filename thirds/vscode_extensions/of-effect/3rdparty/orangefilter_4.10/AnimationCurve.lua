--* This Document is AutoGenerate by OrangeFilter, Don't Change it! *

---@meta

---
---[3.3]animation curve[parent:]
---
---@class AnimationCurve
AnimationCurve = {}

---
---[3.3]constructor, no param
---
--- @nodiscard
function AnimationCurve:new() end

---
---[3.3]add key
---
--- @param arg0 float
--- @param arg1 float
--- @param arg2 float
--- @param arg3 float
--- @nodiscard
function AnimationCurve:addKey(arg0, arg1, arg2, arg3) end
---
---[3.3]evaluate value by time
---
--- @param arg0 float
--- @return float
--- @nodiscard
function AnimationCurve:evaluate(arg0) end

return AnimationCurve
