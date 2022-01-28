--* This Document is AutoGenerate by OrangeFilter, Don't Change it! *

---@meta

---
---[4.3]LocusCameraAnimationData[parent:Serializable]
---
---@class LocusCameraAnimationData
LocusCameraAnimationData = {}

---
---[4.3]constructor, no param
---
--- @nodiscard
function LocusCameraAnimationData:new() end

---
---[4.3]load data from json file
---
--- @param arg0 string
--- @return bool
--- @nodiscard
function LocusCameraAnimationData:loadFromFile(arg0) end
---
---[4.3]clear animtion data
---
--- @nodiscard
function LocusCameraAnimationData:clear() end
---
---[4.3]check if has animtion data
---
--- @return bool
--- @nodiscard
function LocusCameraAnimationData:isEmpty() end
---
---[4.3]get camera type, one or two node
---
--- @return bool
--- @nodiscard
function LocusCameraAnimationData:getCameraType() end
---
---[4.3]get value by time
---
--- @param arg0 float
--- @return CameraKeyFrame#cameraKeyFrame
--- @nodiscard
function LocusCameraAnimationData:getValue(arg0) end

return LocusCameraAnimationData
