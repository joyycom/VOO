--* This Document is AutoGenerate by OrangeFilter, Don't Change it! *

---@meta

---
---[3.2]sprite renderer[parent:]
---
---@class UISpriteRenderer
UISpriteRenderer = {}

---
---[3.2]constructor, 3 param
---
--- @param arg0 Context#context
--- @param arg1 string
--- @param arg2 bool
--- @nodiscard
function UISpriteRenderer:new(arg0, arg1, arg2) end
---
---[4.5]constructor, 4 param
---
--- @param arg0 Context#context
--- @param arg1 string
--- @param arg2 bool
--- @param arg3 int
--- @nodiscard
function UISpriteRenderer:new(arg0, arg1, arg2, arg3) end

---
---[3.2]get screen size
---
--- @return Vec2i#vec2i
--- @nodiscard
function UISpriteRenderer:getScreenSize() end
---
---[3.2]set screen size
---
--- @param arg0 int
--- @param arg1 int
--- @nodiscard
function UISpriteRenderer:setScreenSize(arg0, arg1) end
---
---[3.2]set target size
---
--- @param arg0 int
--- @param arg1 int
--- @nodiscard
function UISpriteRenderer:setTargetSize(arg0, arg1) end
---
---[3.2]get game size
---
--- @return Vec2i#vec2i
--- @nodiscard
function UISpriteRenderer:getGameSize() end
---
---[3.2]get sprite scale
---
--- @return float
--- @nodiscard
function UISpriteRenderer:getSpriteScale() end
---
---[3.2]enable or disable sprite scale
---
--- @param arg0 bool
--- @nodiscard
function UISpriteRenderer:enableSpriteScale(arg0) end
---
---[3.2]set sprite blend mode
---
--- @param arg0 OF_UISpriteBlendMode#of_uispriteblendmode
--- @nodiscard
function UISpriteRenderer:setBlendMode(arg0) end
---
---[3.2]get atlas texture
---
--- @return Texture#texture
--- @nodiscard
function UISpriteRenderer:getAtlasTexture() end
---
---[3.2]draw sprite
---
--- @param arg0 string
--- @param arg1 Matrix4f#matrix4f
--- @param arg2 Color#color
--- @param arg3 Vec4f#vec4f
--- @nodiscard
function UISpriteRenderer:drawSprite(arg0, arg1, arg2, arg3) end
---
---[3.2]draw sprite with texture
---
--- @param arg0 Texture#texture
--- @param arg1 Matrix4f#matrix4f
--- @param arg2 Color#color
--- @param arg3 Vec4f#vec4f
--- @nodiscard
function UISpriteRenderer:drawSpriteWithTexture(arg0, arg1, arg2, arg3) end
---
---[4.5]draw sprite with texture and program
---
--- @param arg0 Texture#texture
--- @param arg1 Program#program
--- @param arg2 Matrix4f#matrix4f
--- @param arg3 Color#color
--- @param arg4 Vec4f#vec4f
--- @nodiscard
function UISpriteRenderer:drawSpriteWithTextureAndProgram(arg0, arg1, arg2, arg3, arg4) end
---
---[3.2]add sprite instance
---
--- @param arg0 Matrix4f#matrix4f
--- @nodiscard
function UISpriteRenderer:addInstance(arg0) end
---
---[4.5]update sprite instance
---
--- @param arg0 int
--- @param arg1 Matrix4f#matrix4f
--- @nodiscard
function UISpriteRenderer:updateInstance(arg0, arg1) end
---
---[3.2]clear all sprite instance
---
--- @nodiscard
function UISpriteRenderer:clearInstances() end
---
---[3.2]draw sprite instances
---
--- @param arg0 string
--- @param arg1 int
--- @param arg2 int
--- @param arg3 Color#color
--- @param arg4 Vec4f#vec4f
--- @nodiscard
function UISpriteRenderer:drawSpriteInstanced(arg0, arg1, arg2, arg3, arg4) end
---
---[3.2]add sprite
---
--- @param arg0 string
--- @param arg1 int
--- @param arg2 int
--- @param arg3 int
--- @param arg4 int
--- @param arg5 int
--- @param arg6 int
--- @param arg7 float
--- @param arg8 Vec2f#vec2f
--- @param arg9 Color#color
--- @return int
--- @nodiscard
function UISpriteRenderer:addSprite(arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9) end
---
---[3.2]set sprite color
---
--- @param arg0 int
--- @param arg1 Color#color
--- @nodiscard
function UISpriteRenderer:setSpriteColor(arg0, arg1) end
---
---[3.2]set sprite visible
---
--- @param arg0 int
--- @param arg1 bool
--- @nodiscard
function UISpriteRenderer:setSpriteVisible(arg0, arg1) end
---
---[3.2]is sprite visible
---
--- @param arg0 int
--- @return bool
--- @nodiscard
function UISpriteRenderer:isSpriteVisible(arg0) end
---
---[3.2]get sprite position
---
--- @param arg0 int
--- @return Vec2f#vec2f
--- @nodiscard
function UISpriteRenderer:getSpritePos(arg0) end
---
---[3.2]set sprite position
---
--- @param arg0 int
--- @param arg1 float
--- @param arg2 float
--- @nodiscard
function UISpriteRenderer:setSpritePos(arg0, arg1, arg2) end
---
---[3.2]get sprite size
---
--- @param arg0 int
--- @return Vec2i#vec2i
--- @nodiscard
function UISpriteRenderer:getSpriteSize(arg0) end
---
---[3.2]set sprite size
---
--- @param arg0 int
--- @param arg1 int
--- @param arg2 int
--- @nodiscard
function UISpriteRenderer:setSpriteSize(arg0, arg1, arg2) end
---
---[3.2]set sprite name
---
--- @param arg0 int
--- @param arg1 string
--- @nodiscard
function UISpriteRenderer:setSpriteName(arg0, arg1) end
---
---[3.2]set sprite uv scale offset
---
--- @param arg0 int
--- @param arg1 Vec4f#vec4f
--- @nodiscard
function UISpriteRenderer:setSpriteUVScaleOffset(arg0, arg1) end
---
---[3.2]remove sprite
---
--- @param arg0 int
--- @nodiscard
function UISpriteRenderer:removeSprite(arg0) end
---
---[3.2]clear all sprite
---
--- @nodiscard
function UISpriteRenderer:clearSprite() end
---
---[3.2]update sprites
---
--- @nodiscard
function UISpriteRenderer:updateSprites() end
---
---[3.2]draw sprites
---
--- @nodiscard
function UISpriteRenderer:drawSprites() end
---
---[4.5]draw sprites with mirror
---
--- @param arg0 bool
--- @nodiscard
function UISpriteRenderer:drawSpritesMirror(arg0) end

---
---[3.2]build sprite matrix for render
---
--- @param arg0 Matrix4f#matrix4f
--- @param arg1 int
--- @param arg2 int
--- @param arg3 int
--- @param arg4 int
--- @param arg5 int
--- @param arg6 int
--- @param arg7 float
--- @param arg8 Vec2f#vec2f
--- @nodiscard
function UISpriteRenderer.BuildSpriteMatrix(arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8) end

return UISpriteRenderer
