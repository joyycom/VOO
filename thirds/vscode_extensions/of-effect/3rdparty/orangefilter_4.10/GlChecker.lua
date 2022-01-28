--* This Document is AutoGenerate by OrangeFilter, Don't Change it! *

---@meta

---
---[4.10]gl info checker[parent:]
---
---@class GlChecker
GlChecker = {}

---
---[4.10]glVersion
---
--- @return int
--- @nodiscard
function GlChecker:glVersion() end
---
---[4.10]glslVersion
---
--- @return int
--- @nodiscard
function GlChecker:glslVersion() end
---
---[4.10]if Support Draw Instance
---
--- @return bool
--- @nodiscard
function GlChecker:isSupportDrawInstanced() end
---
---[4.10]if Support Draw Instance
---
--- @param arg0 string
--- @param arg1 string
--- @return bool
--- @nodiscard
function GlChecker:isSupportExtension(arg0, arg1) end
---
---[4.10]if Support Pixel Buffer
---
--- @return bool
--- @nodiscard
function GlChecker:isSupportPixelBuffer() end
---
---[4.10]get max Vertex Texture Image Unit
---
--- @return int
--- @nodiscard
function GlChecker:maxVertexTextureImageUnit() end

return GlChecker
