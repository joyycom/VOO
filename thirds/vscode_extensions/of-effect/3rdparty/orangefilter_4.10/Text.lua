--* This Document is AutoGenerate by OrangeFilter, Don't Change it! *

---@meta

---
---[4.0]Text[parent:]
---
---@class Text
Text = {}

---
---[4.0]constructor, 1 param
---
--- @param arg0 Context#context
--- @nodiscard
function Text:new(arg0) end

---
---[4.0]set font
---
--- @param arg0 string
--- @param arg1 float
--- @param arg2 string
--- @nodiscard
function Text:setFont(arg0, arg1, arg2) end
---
---[4.2]set font
---
--- @param arg0 string
--- @param arg1 float
--- @param arg2 int
--- @param arg3 string
--- @nodiscard
function Text:setFontWithOutline(arg0, arg1, arg2, arg3) end
---
---[4.5]set font
---
--- @param arg0 string
--- @param arg1 float
--- @param arg2 int
--- @param arg3 bool
--- @param arg4 string
--- @nodiscard
function Text:setFontWithSDF(arg0, arg1, arg2, arg3, arg4) end
---
---[4.0]set string
---
--- @param arg0 string
--- @nodiscard
function Text:setString(arg0) end
---
---[4.0]get texture
---
--- @return Texture#texture
--- @nodiscard
function Text:getTexture() end
---
---[4.0]set color
---
--- @param arg0 Vec3us
--- @nodiscard
function Text:setColor(arg0) end
---
---[4.0]set alpha
---
--- @param arg0 OFUInt8
--- @nodiscard
function Text:setAlpha(arg0) end
---
---[4.0]set size
---
--- @param arg0 int
--- @nodiscard
function Text:setFontSize(arg0) end
---
---[4.0]set bold
---
--- @param arg0 bool
--- @nodiscard
function Text:setBold(arg0) end
---
---[4.0]set shadow
---
--- @param arg0 bool
--- @nodiscard
function Text:setShadow(arg0) end
---
---[4.7]set shadow blur
---
--- @param arg0 float
--- @nodiscard
function Text:setShadowBlur(arg0) end
---
---[4.7]set shadow offset
---
--- @param arg0 float
--- @param arg1 float
--- @nodiscard
function Text:setShadowOffset(arg0, arg1) end
---
---[4.7]set shadow opacity
---
--- @param arg0 float
--- @nodiscard
function Text:setShadowOpacity(arg0) end
---
---[4.0]set spacing
---
--- @param arg0 int
--- @nodiscard
function Text:setSpacing(arg0) end
---
---[4.1]set direction
---
--- @param arg0 int
--- @nodiscard
function Text:setDirection(arg0) end
---
---[4.1]set alignment
---
--- @param arg0 OF_TextAlign#of_textalign
--- @nodiscard
function Text:setAlignment(arg0) end
---
---[4.2]set outline color
---
--- @param arg0 Vec3us
--- @nodiscard
function Text:setOutlineColor(arg0) end
---
---[4.5]set outline alpha
---
--- @param arg0 OFUInt8
--- @nodiscard
function Text:setOutlineAlpha(arg0) end
---
---[4.2]set leading
---
--- @param arg0 int
--- @nodiscard
function Text:setLeading(arg0) end
---
---[4.4]get raster positions
---
--- @param arg0 int
--- @param arg1 int
--- @param arg2 int
--- @param arg3 int
--- @param arg4 IntArray#intarray
--- @param arg5 int
--- @return int
--- @nodiscard
function Text:getRasterPositions(arg0, arg1, arg2, arg3, arg4, arg5) end
---
---[4.6]get glyph count
---
--- @return int
--- @nodiscard
function Text:getGlyphCount() end
---
---[4.6]get glyph outline
---
--- @param arg0 int
--- @return GlyphOutline
--- @nodiscard
function Text:getGlyphOutline(arg0) end
---
---[4.7]apply
---
--- @param arg0 OF_Texture#of_texture
--- @param arg1 Matrix4f#matrix4f
--- @nodiscard
function Text:apply(arg0, arg1) end

return Text
