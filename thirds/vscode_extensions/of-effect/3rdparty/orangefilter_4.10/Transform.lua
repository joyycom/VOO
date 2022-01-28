--* This Document is AutoGenerate by OrangeFilter, Don't Change it! *

---@meta

---
---[3.6]transform[parent:Component#component]
---
---@class Transform
Transform = {}

---
---[3.6]get parent transform
---
--- @return Transform#transform
--- @nodiscard
function Transform:getParent() end
---
---[3.6]set parent transform
---
--- @param arg0 Transform#transform
--- @nodiscard
function Transform:setParent(arg0) end
---
---[3.6]get child count
---
--- @return int
--- @nodiscard
function Transform:getChildCount() end
---
---[3.6]get child
---
--- @param arg0 int
--- @return Transform#transform
--- @nodiscard
function Transform:getChild(arg0) end
---
---[3.6]find child by path
---
--- @param arg0 string
--- @return Transform#transform
--- @nodiscard
function Transform:find(arg0) end
---
---[3.6]get local position
---
--- @return Vec3f
--- @nodiscard
function Transform:getLocalPosition() end
---
---[3.6]set local position
---
--- @param arg0 Vec3f
--- @nodiscard
function Transform:setLocalPosition(arg0) end
---
---[4.1]set local position offset
---
--- @param arg0 Vec3f
--- @nodiscard
function Transform:setLocalPositionOffset(arg0) end
---
---[3.6]get local rotation
---
--- @return Quaternion
--- @nodiscard
function Transform:getLocalRotation() end
---
---[3.6]set local rotation
---
--- @param arg0 Quaternion
--- @nodiscard
function Transform:setLocalRotation(arg0) end
---
---[4.1]set local rotation offset
---
--- @param arg0 Quaternion
--- @nodiscard
function Transform:setLocalRotationOffset(arg0) end
---
---[3.6]get local scale
---
--- @return Vec3f
--- @nodiscard
function Transform:getLocalScale() end
---
---[3.6]set local scale
---
--- @param arg0 Vec3f
--- @nodiscard
function Transform:setLocalScale(arg0) end
---
---[4.1]set local scale offset
---
--- @param arg0 Vec3f
--- @nodiscard
function Transform:setLocalScaleOffset(arg0) end
---
---[3.6]get world position
---
--- @return Vec3f
--- @nodiscard
function Transform:getPosition() end
---
---[3.6]set world position
---
--- @param arg0 Vec3f
--- @nodiscard
function Transform:setPosition(arg0) end
---
---[3.6]get world rotation
---
--- @return Quaternion
--- @nodiscard
function Transform:getRotation() end
---
---[3.6]set world rotation
---
--- @param arg0 Quaternion
--- @nodiscard
function Transform:setRotation(arg0) end
---
---[3.6]get world scale
---
--- @return Vec3f
--- @nodiscard
function Transform:getScale() end
---
---[3.6]set world scale
---
--- @param arg0 Vec3f
--- @nodiscard
function Transform:setScale(arg0) end
---
---[3.6]get local to world matrix
---
--- @return Matrix4f
--- @nodiscard
function Transform:getLocalToWorldMatrix() end
---
---[3.6]get world to local matrix
---
--- @return Matrix4f
--- @nodiscard
function Transform:getWorldToLocalMatrix() end

return Transform
