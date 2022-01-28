--* This Document is AutoGenerate by OrangeFilter, Don't Change it! *

---@meta

---
---[3.2]texture[parent:]
---
---@class Texture
Texture = {}

---
---[4.10]increase tex reference
---
--- @nodiscard
function Texture:incRefCount() end
---
---[4.10]decrease tex reference
---
--- @nodiscard
function Texture:decRefCount() end
---
---[3.6]get width
---
--- @return int
--- @nodiscard
function Texture:width() end
---
---[3.6]get height
---
--- @return int
--- @nodiscard
function Texture:height() end
---
---[3.2]to OF_Texture data
---
--- @return OF_Texture#of_texture
--- @nodiscard
function Texture:toOFTexture() end
---
---[3.2]get gl texture id
---
--- @return int
--- @nodiscard
function Texture:textureID() end
---
---[4.5]get memory data copy
---
--- @param arg0 ByteArray#bytearray
--- @param arg1 int
--- @param arg2 int
--- @param arg3 int
--- @nodiscard
function Texture:getDataCopy(arg0, arg1, arg2, arg3) end
---
---[3.2]gen mipmap levels
---
--- @nodiscard
function Texture:genMipmap() end
---
---[4.1]bind fbo
---
--- @param arg0 int
--- @return int
--- @nodiscard
function Texture:bindFBO(arg0) end
---
---[4.6]get frame times, for multi-frame texture like gif
---
--- @return int[]
--- @nodiscard
function Texture:getFrameTimes() end
---
---[4.6]update frame, for multi-frame texture like gif
---
--- @param arg0 int
--- @nodiscard
function Texture:updateFrame(arg0) end
---
---[4.4]copy text to memory
---
--- @param arg0 ByteArray#bytearray
--- @param arg1 int
--- @param arg2 int
--- @return bool
--- @nodiscard
function Texture:copyToMemory(arg0, arg1, arg2) end

return Texture
