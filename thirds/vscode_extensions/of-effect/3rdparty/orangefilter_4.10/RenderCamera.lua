--* This Document is AutoGenerate by OrangeFilter, Don't Change it! *

---@meta

---
---[3.6]camera[parent:Component#component]
---
---@class RenderCamera
RenderCamera = {}

---
---[3.6]set clear mode
---
--- @param arg0 OF_CameraClearFlags#of_cameraclearflags
--- @nodiscard
function RenderCamera:setClearFlags(arg0) end
---
---[3.6]set clear color
---
--- @param arg0 Color
--- @nodiscard
function RenderCamera:setClearColor(arg0) end
---
---[3.6]get culling mask
---
--- @return int
--- @nodiscard
function RenderCamera:getCullingMask() end
---
---[3.6]set culling mask
---
--- @param arg0 int
--- @nodiscard
function RenderCamera:setCullingMask(arg0) end
---
---[3.6]set orthographic
---
--- @param arg0 bool
--- @nodiscard
function RenderCamera:setOrthographic(arg0) end
---
---[3.6]set orthographic size
---
--- @param arg0 float
--- @nodiscard
function RenderCamera:setOrthographicSize(arg0) end
---
---[3.6]set fov
---
--- @param arg0 float
--- @nodiscard
function RenderCamera:setFieldOfView(arg0) end
---
---[3.6]set near clip
---
--- @param arg0 float
--- @nodiscard
function RenderCamera:setClipNear(arg0) end
---
---[3.6]set far clip
---
--- @param arg0 float
--- @nodiscard
function RenderCamera:setClipFar(arg0) end
---
---[3.6]set viewport rect
---
--- @param arg0 float
--- @param arg1 float
--- @param arg2 float
--- @param arg3 float
--- @nodiscard
function RenderCamera:setViewportRect(arg0, arg1, arg2, arg3) end
---
---[3.6]set depth, camera render order is sort by this, less first
---
--- @param arg0 int
--- @nodiscard
function RenderCamera:setDepth(arg0) end
---
---[4.7]set depth, camera render order is sort by this, less first
---
--- @param arg0 int
--- @nodiscard
function RenderCamera:setDebugDraw(arg0) end
---
---[3.6]get target width
---
--- @return int
--- @nodiscard
function RenderCamera:getTargetWidth() end
---
---[3.6]get target height
---
--- @return int
--- @nodiscard
function RenderCamera:getTargetHeight() end
---
---[3.6]set target size
---
--- @param arg0 int
--- @param arg1 int
--- @nodiscard
function RenderCamera:setTargetSize(arg0, arg1) end
---
---[4.1]set render target texture
---
--- @param arg0 Texture#texture
--- @param arg1 Texture#texture
--- @nodiscard
function RenderCamera:setRenderTarget(arg0, arg1) end
---
---[4.1]set replacement shader for this camera rendering
---
--- @param arg0 string
--- @nodiscard
function RenderCamera:setReplacementShader(arg0) end
---
---[4.6]set replacement render state for this camera rendering
---
--- @param arg0 RenderState#renderstate
--- @nodiscard
function RenderCamera:setReplacementRenderState(arg0) end
---
---[4.1]set view space to left hand (same with unity
---
--- @param arg0 bool
--- @nodiscard
function RenderCamera:setLeftHandSpace(arg0) end
---
---[4.5]set view matrix external
---
--- @param arg0 FloatArray#floatarray
--- @nodiscard
function RenderCamera:setViewMatrixExternal(arg0) end
---
---[4.5]set projection matrix external
---
--- @param arg0 FloatArray#floatarray
--- @nodiscard
function RenderCamera:setProjectionMatrixExternal(arg0) end
---
---[3.6]get view matrix
---
--- @return Matrix4f
--- @nodiscard
function RenderCamera:getViewMatrix() end
---
---[3.6]get projection matrix
---
--- @return Matrix4f
--- @nodiscard
function RenderCamera:getProjectionMatrix() end
---
---[3.6]get view-projection matrix
---
--- @return Matrix4f
--- @nodiscard
function RenderCamera:getViewProjectionMatrix() end

return RenderCamera
