--* This Document is AutoGenerate by OrangeFilter, Don't Change it! *

---@meta

---
---[3.2]render tree node,used to render ofmodel[parent:Node#node]
---
---@class RendererTreeNode
RendererTreeNode = {}

---
---[3.2]load `.ofmodel` or `.obj`
---
--- @param arg0 string
--- @nodiscard
function RendererTreeNode:loadModel(arg0) end
---
---[3.2]copy node,share mesh and material data with source node
---
--- @param arg0 RendererTreeNode#renderertreenode
--- @nodiscard
function RendererTreeNode:copy(arg0) end
---
---[3.2]set frame time and delta time
---
--- @param arg0 float
--- @param arg1 float
--- @nodiscard
function RendererTreeNode:setFrameTimes(arg0, arg1) end
---
---[3.2]update node
---
--- @param arg0 float
--- @nodiscard
function RendererTreeNode:update(arg0) end
---
---[3.2]render node
---
--- @param arg0 Context#context
--- @nodiscard
function RendererTreeNode:render(arg0) end
---
---[3.2]set position
---
--- @param arg0 Vec3f#vec3f
--- @nodiscard
function RendererTreeNode:setPosition(arg0) end
---
---[3.2]get position
---
--- @return Vec3f#vec3f
--- @nodiscard
function RendererTreeNode:getPosition() end
---
---[3.2]set rotation
---
--- @param arg0 Quaternion#quaternion
--- @nodiscard
function RendererTreeNode:setRotation(arg0) end
---
---[3.2]get rotation
---
--- @return Quaternion#quaternion
--- @nodiscard
function RendererTreeNode:getRotation() end
---
---[3.2]set scale
---
--- @param arg0 float
--- @nodiscard
function RendererTreeNode:setScale(arg0) end
---
---[3.2]get scale
---
--- @return float
--- @nodiscard
function RendererTreeNode:getScale() end
---
---[3.2]get submesh indices
---
--- @return int[]
--- @nodiscard
function RendererTreeNode:getSubmeshIndices() end
---
---[3.2]set submesh visible
---
--- @param arg0 int
--- @param arg1 bool
--- @nodiscard
function RendererTreeNode:setSubmeshVisible(arg0, arg1) end
---
---[3.2]get material
---
--- @return Material#material
--- @nodiscard
function RendererTreeNode:getMaterial() end
---
---[3.6]get mesh
---
--- @return MeshLegacy#meshlegacy
--- @nodiscard
function RendererTreeNode:getMesh() end
---
---[3.4]get animation clip names
---
--- @return string[]
--- @nodiscard
function RendererTreeNode:getAnimationClipNames() end
---
---[3.4]play animation clip
---
--- @param arg0 string
--- @param arg1 OF_AnimationWrapMode#OF_animationwrapmode
--- @nodiscard
function RendererTreeNode:playAnimation(arg0, arg1) end
---
---[3.3]get particle systems
---
--- @return ParticleSystemRendererNodeLegacy#particlesystemrenderernodelegacy)[]
--- @nodiscard
function RendererTreeNode:getParticleSystems() end
---
---[3.2]play particle systems
---
--- @nodiscard
function RendererTreeNode:playParticleSystems() end
---
---[3.2]pause particle systems
---
--- @nodiscard
function RendererTreeNode:pauseParticleSystems() end
---
---[3.2]stop particle systems
---
--- @nodiscard
function RendererTreeNode:stopParticleSystems() end
---
---[3.2]restart particle systems
---
--- @nodiscard
function RendererTreeNode:restartParticleSystems() end
---
---[3.2]enable particle systems instance render mode
---
--- @nodiscard
function RendererTreeNode:enableParticleSystemsInstanceRenderMode() end
---
---[3.2]get submesh bounding box
---
--- @param arg0 int
--- @return BoundingBox#boundingbox
--- @nodiscard
function RendererTreeNode:getSubmeshBoundingBox(arg0) end
---
---[3.3]get bounding box
---
--- @return BoundingBox#boundingbox
--- @nodiscard
function RendererTreeNode:getBoundingBox() end
---
---[3.2]get root node world matrix
---
--- @return Matrix4f#matrix4f
--- @nodiscard
function RendererTreeNode:rootWorldMatrix() end
---
---[3.3]set extension world matrix
---
--- @param arg0 Matrix4f#matrix4f
--- @nodiscard
function RendererTreeNode:setWorldMatrixExt(arg0) end
---
---[3.6]get blend shape count
---
--- @return int
--- @nodiscard
function RendererTreeNode:getBlendShapeCount() end
---
---[3.6]get blend shape name
---
--- @param arg0 int
--- @return string
--- @nodiscard
function RendererTreeNode:getBlendShapeName(arg0) end
---
---[3.6]get blend shape weight
---
--- @param arg0 int
--- @return float
--- @nodiscard
function RendererTreeNode:getBlendShapeWeight(arg0) end
---
---[3.6]set blend shape weight
---
--- @param arg0 int
--- @param arg1 float
--- @nodiscard
function RendererTreeNode:setBlendShapeWeight(arg0, arg1) end
---
---[3.6]get root renderer node
---
--- @return RendererNode#renderernode
--- @nodiscard
function RendererTreeNode:getRootNode() end

---
---[3.3]begin particle system batch rendering
---
--- @param arg0 Context#context
--- @param arg1 Material#material
--- @param arg2 int
--- @nodiscard
function RendererTreeNode.beginParticleSystemBatchRender(arg0, arg1, arg2) end
---
---[3.3]end particle system batch rendering
---
--- @param arg0 Context#context
--- @nodiscard
function RendererTreeNode.endParticleSystemBatchRender(arg0) end
---
---[3.3]reset particle system batch rendering
---
--- @param arg0 Context#context
--- @nodiscard
function RendererTreeNode.resetParticleSystemBatchRender(arg0) end

return RendererTreeNode
