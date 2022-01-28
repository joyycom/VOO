--* This Document is AutoGenerate by OrangeFilter, Don't Change it! *

---@meta

---
---[3.2]for restore gl render states[parent:]
---
---@class RenderStatesRestorer
RenderStatesRestorer = {}

---
---[3.2]constructor, no param
---
--- @nodiscard
function RenderStatesRestorer:new() end

---
---[3.3]save current render states
---
--- @nodiscard
function RenderStatesRestorer:save() end
---
---[3.3]restore previous render states
---
--- @nodiscard
function RenderStatesRestorer:restore() end

return RenderStatesRestorer
