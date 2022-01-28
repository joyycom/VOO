--* This Document is AutoGenerate by OrangeFilter, Don't Change it! *

---@meta

---
---[3.2]SVGA[parent:BaseFrameHandler]
---
---@class SVGA
SVGA = {}

---
---[3.2]load svga from file
---
--- @param arg0 string
--- @param arg1 bool
--- @return bool
--- @nodiscard
function SVGA:load(arg0, arg1) end
---
---[3.2]is resources loaded complete
---
--- @return bool
--- @nodiscard
function SVGA:isLoaded() end
---
---[3.2]get view width
---
--- @return int
--- @nodiscard
function SVGA:viewWidth() end
---
---[3.2]get view height
---
--- @return int
--- @nodiscard
function SVGA:viewHeight() end
---
---[3.2]get frame count
---
--- @return int
--- @nodiscard
function SVGA:frames() end
---
---[3.2]get fps
---
--- @return int
--- @nodiscard
function SVGA:fps() end
---
---[3.5]set fixed texture max size
---
--- @param arg0 int
--- @nodiscard
function SVGA:setFixedTextureMaxSize(arg0) end
---
---[3.2]render svga to texture
---
--- @param arg0 OF_Texture#of_texture
--- @param arg1 int
--- @return OF_Result#of_result
--- @nodiscard
function SVGA:applyRGBA(arg0, arg1) end
---
---[4.0]render svga to a bounding texture
---
--- @param arg0 int
--- @param arg1 canvas width
--- @param arg2 canvas height
--- @return [OF_Result#of_result)
--- @nodiscard
function SVGA:render(arg0, arg1, arg2) end
---
---[4.0]set svga transforms
---
--- @param arg0 float
--- @param arg1 float
--- @param arg2 float
--- @param arg3 float
--- @param arg4 float
--- @nodiscard
function SVGA:setRect(arg0, arg1, arg2, arg3, arg4) end
---
---[4.5]get sprite transform in frame
---
--- @param arg0 string
--- @param arg1 int
--- @param arg2 FloatArray#floatarray
--- @return OF_Result#of_result
--- @nodiscard
function SVGA:getSpriteTransform(arg0, arg1, arg2) end
---
---[4.5]set sprite visible
---
--- @param arg0 string
--- @param arg1 bool
--- @return OF_Result#of_result
--- @nodiscard
function SVGA:setSpriteVisible(arg0, arg1) end
---
---[4.5]get sprite alpha in frame
---
--- @param arg0 string
--- @param arg1 int
--- @return float
--- @nodiscard
function SVGA:getSpriteAlpha(arg0, arg1) end
---
---[4.5]set image for sprite
---
--- @param arg0 string
--- @param arg1 string
--- @return bool
--- @nodiscard
function SVGA:setImageForSprite(arg0, arg1) end
---
---[4.5]enable render to output direct
---
--- @param arg0 bool
--- @nodiscard
function SVGA:enableRenderToOuputDirect(arg0) end
---
---[4.8]set image for sprite
---
--- @param arg0 string
--- @param arg1 Texture#texture
--- @return bool
--- @nodiscard
function SVGA:setTextureForSprite(arg0, arg1) end

return SVGA
