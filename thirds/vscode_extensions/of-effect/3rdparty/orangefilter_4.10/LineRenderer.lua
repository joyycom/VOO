--* This Document is AutoGenerate by OrangeFilter, Don't Change it! *

---@meta

---
---[3.2]line renderer[parent:]
---
---@class LineRenderer
LineRenderer = {}

---
---[3.2]update renderer
---
--- @nodiscard
function LineRenderer:update() end
---
---[3.2]render this
---
--- @nodiscard
function LineRenderer:render() end
---
---[3.2]set shader
---
--- @param arg0 string
--- @nodiscard
function LineRenderer:setShader(arg0) end
---
---[3.2]set points
---
--- @param arg0 Vec3f#vec3f)[]
--- @nodiscard
function LineRenderer:setPoints(arg0) end
---
---[3.2]set line width
---
--- @param arg0 float
--- @nodiscard
function LineRenderer:setLineWidth(arg0) end
---
---[3.2]set line color
---
--- @param arg0 Color#color
--- @nodiscard
function LineRenderer:setColor(arg0) end
---
---[3.2]set texture
---
--- @param arg0 Texture#texture
--- @param arg1 Vec4f#vec4f
--- @nodiscard
function LineRenderer:setTexture(arg0, arg1) end

return LineRenderer
