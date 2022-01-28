--* This Document is AutoGenerate by OrangeFilter, Don't Change it! *

---@meta

---
---[4.9]particle system , used to manager particle[parent:]
---
---@class ParticleEditorSystem
ParticleEditorSystem = {}

---
---[4.9]destory particle system
---
--- @nodiscard
function ParticleEditorSystem:destroy() end
---
---[4.9]start particle system
---
--- @nodiscard
function ParticleEditorSystem:start() end
---
---[4.9]stop particle system
---
--- @nodiscard
function ParticleEditorSystem:stop() end
---
---[4.9]reset particle system
---
--- @nodiscard
function ParticleEditorSystem:reset() end
---
---[4.9]judge particle system is run
---
--- @return bool
--- @nodiscard
function ParticleEditorSystem:isActive() end
---
---[4.9]judge particle system is full
---
--- @return bool
--- @nodiscard
function ParticleEditorSystem:isFull() end
---
---[4.9]get particle system last time
---
--- @return float
--- @nodiscard
function ParticleEditorSystem:getDuration() end
---
---[4.9]set particle system last time
---
--- @param arg0 float
--- @nodiscard
function ParticleEditorSystem:setDuration(arg0) end
---
---[4.9]get particle system emitter
---
--- @return ParticleEditorEmitter#particlemitter
--- @nodiscard
function ParticleEditorSystem:emitter() end
---
---[4.9]get particle system renderer
---
--- @return ParticleEditorRender#particlerenderer
--- @nodiscard
function ParticleEditorSystem:renderer() end
---
---[4.9]update node
---
--- @param arg0 float
--- @nodiscard
function ParticleEditorSystem:update(arg0) end
---
---[4.9]set position
---
--- @param arg0 Vec3f#vec3f
--- @nodiscard
function ParticleEditorSystem:setPosition(arg0) end

---
---[4.9]create particleSystem
---
--- @return ParticleSystem#particleSystem
--- @nodiscard
function ParticleEditorSystem.create() end

return ParticleEditorSystem
