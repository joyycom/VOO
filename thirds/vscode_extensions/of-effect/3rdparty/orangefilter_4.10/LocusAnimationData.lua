--* This Document is AutoGenerate by OrangeFilter, Don't Change it! *

---@meta

---
---[4.0]LocusAnimationData[parent:Serializable]
---
---@class LocusAnimationData
LocusAnimationData = {}

---
---[4.0]constructor, no param
---
--- @nodiscard
function LocusAnimationData:new() end

---
---[4.0]load data from json file
---
--- @param arg0 string
--- @return bool
--- @nodiscard
function LocusAnimationData:loadFromFile(arg0) end
---
---[4.0]load data from json file
---
--- @nodiscard
function LocusAnimationData:clear() end
---
---[4.0]get value by time
---
--- @param arg0 float
--- @return LocusKeyFrame#locuskeyframe
--- @nodiscard
function LocusAnimationData:getValue(arg0) end

return LocusAnimationData
