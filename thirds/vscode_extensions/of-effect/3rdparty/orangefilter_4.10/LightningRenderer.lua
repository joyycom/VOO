--* This Document is AutoGenerate by OrangeFilter, Don't Change it! *

---@meta

---
---[3.2]LightningRenderer[parent:]
---
---@class LightningRenderer
LightningRenderer = {}

---
---[3.2]update renderer
---
--- @nodiscard
function LightningRenderer:update() end
---
---[3.2]render this
---
--- @nodiscard
function LightningRenderer:render() end
---
---[3.2]set begin position
---
--- @param arg0 Vec3f#vec3f
--- @nodiscard
function LightningRenderer:setBegin(arg0) end
---
---[3.2]set end position
---
--- @param arg0 Vec3f#vec3f
--- @nodiscard
function LightningRenderer:setEnd(arg0) end
---
---[3.2]set lightning offset
---
--- @param arg0 Vec3f#vec3f
--- @nodiscard
function LightningRenderer:setOffset(arg0) end
---
---[3.2]set lightning depth
---
--- @param arg0 int
--- @nodiscard
function LightningRenderer:setDepth(arg0) end
---
---[3.2]get line renderer
---
--- @return LineRenderer#linerenderer
--- @nodiscard
function LightningRenderer:getLineRenderer() end

return LightningRenderer
