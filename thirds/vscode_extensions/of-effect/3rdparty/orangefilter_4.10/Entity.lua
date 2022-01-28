--* This Document is AutoGenerate by OrangeFilter, Don't Change it! *

---@meta

---
---[3.6]entity, can attach multi-components[parent:Object#object]
---
---@class Entity
Entity = {}

---
---[3.6]get camera component
---
--- @return RenderCamera#rendercamera
--- @nodiscard
function Entity:getComponentRenderCamera() end
---
---[3.6]get animation component
---
--- @return Animation#animation
--- @nodiscard
function Entity:getComponentAnimation() end
---
---[3.7]get mesh renderer component
---
--- @return MeshRenderer#meshrenderer
--- @nodiscard
function Entity:getComponentMeshRenderer() end
---
---[3.7]get skinned mesh renderer component
---
--- @return SkinnedMeshRenderer#skinnedmeshrenderer
--- @nodiscard
function Entity:getComponentSkinnedMeshRenderer() end
---
---[4.7]get cloth component
---
--- @return Cloth#cloth
--- @nodiscard
function Entity:getComponentCloth() end
---
---[4.6]get text mesh renderer component
---
--- @return TextMeshRenderer#textmeshrenderer
--- @nodiscard
function Entity:getComponentTextMeshRenderer() end
---
---[4.2]get particle system component
---
--- @return ParticleSystem#particlesystem
--- @nodiscard
function Entity:getComponentParticleSystem() end
---
---[4.1]get all renderers component in entity and children
---
--- @return Renderer#renderer)[]
--- @nodiscard
function Entity:getComponentsInChildrenRenderer() end
---
---[4.1]get all skinned mesh renderers component in entity and children
---
--- @return SkinnedMeshRenderer#skinnedmeshrenderer)[]
--- @nodiscard
function Entity:getComponentsInChildrenSkinnedMeshRenderer() end
---
---[3.6]get transform
---
--- @return Transform#transform
--- @nodiscard
function Entity:getTransform() end
---
---[3.6]remove component
---
--- @param arg0 Component#component
--- @nodiscard
function Entity:removeComponent(arg0) end
---
---[3.6]set active state
---
--- @param arg0 bool
--- @nodiscard
function Entity:setActive(arg0) end
---
---[3.6]is active self
---
--- @return bool
--- @nodiscard
function Entity:isActiveSelf() end
---
---[3.6]is active in tree
---
--- @return bool
--- @nodiscard
function Entity:isActiveInTree() end
---
---[3.6]get layer
---
--- @return int
--- @nodiscard
function Entity:getLayer() end
---
---[3.6]set layer
---
--- @param arg0 int
--- @nodiscard
function Entity:setLayer(arg0) end
---
---[3.6]set layer recursively
---
--- @param arg0 int
--- @nodiscard
function Entity:setLayerRecursively(arg0) end

---
---[3.6]create entity
---
--- @param arg0 string
--- @return Entity#entity
--- @nodiscard
function Entity.Create(arg0) end
---
---[3.6]destroy entity
---
--- @param arg0 Entity#entity
--- @nodiscard
function Entity.Destroy(arg0) end

return Entity
