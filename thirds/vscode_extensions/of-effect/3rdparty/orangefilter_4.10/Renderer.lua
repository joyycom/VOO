--* This Document is AutoGenerate by OrangeFilter, Don't Change it! *

---@meta

---
---[3.7]renderer base class[parent:Component#component]
---
---@class Renderer
Renderer = {}

---
---[3.7]get material
---
--- @return Material#material
--- @nodiscard
function Renderer:getMaterial() end
---
---[4.3]get materials
---
--- @return Material#material)[]
--- @nodiscard
function Renderer:getMaterials() end
---
---[4.5]draw this renderer in camera with material, for custom draw
---
--- @param arg0 RenderCamera#rendercamera
--- @param arg1 Material#material
--- @param arg2 int
--- @param arg3 int
--- @nodiscard
function Renderer:drawInCamera(arg0, arg1, arg2, arg3) end

return Renderer
