--* This Document is AutoGenerate by OrangeFilter, Don't Change it! *

---@meta

---
---[4.9]particle renderer , used to render particle[parent:]
---
---@class ParticleEditorRender
ParticleEditorRender = {}

---
---[4.9]rendered particle texture image
---
--- @param arg0 OFInt32]
--- @param arg1 OFInt32]
--- @nodiscard
function ParticleEditorRender:setCanvasSize(arg0, arg1) end
---
---[4.9]rendered particle texture image
---
--- @param arg0 Context#context
--- @param arg1 Program#program
--- @nodiscard
function ParticleEditorRender:draw(arg0, arg1) end
---
---[4.9]rendered particle texture image
---
--- @param arg0 Context#context
--- @param arg1 Program#program
--- @nodiscard
function ParticleEditorRender:drawInstance(arg0, arg1) end

return ParticleEditorRender
