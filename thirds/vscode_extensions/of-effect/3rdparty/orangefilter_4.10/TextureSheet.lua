--* This Document is AutoGenerate by OrangeFilter, Don't Change it! *

---@meta

---
---[4.0]texture animation sheet using png file list[parent:]
---
---@class TextureSheet
TextureSheet = {}

---
---[4.0]clear
---
--- @nodiscard
function TextureSheet:clear() end
---
---[4.0]load image animation sequence
---
--- @param arg0 string[]
--- @param arg1 string
--- @param arg2 int
--- @param arg3 bool
--- @return bool
--- @nodiscard
function TextureSheet:load(arg0, arg1, arg2, arg3) end
---
---[4.0]is loaded or not
---
--- @return bool
--- @nodiscard
function TextureSheet:isLoaded() end
---
---[4.0]get frame texture
---
--- @param arg0 int
--- @return Texture#txture
--- @nodiscard
function TextureSheet:frame(arg0) end
---
---[4.0]return fps
---
--- @return int
--- @nodiscard
function TextureSheet:FPS() end
---
---[4.0]return frames
---
--- @return int
--- @nodiscard
function TextureSheet:frames() end
---
---[4.0]set frame interval
---
--- @nodiscard
function TextureSheet:setFrameInterval() end

return TextureSheet
