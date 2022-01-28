--* This Document is AutoGenerate by OrangeFilter, Don't Change it! *

---@meta

---
---[4.6]glyph outline[parent:]
---
---@class GlyphOutline
GlyphOutline = {}

---
---[4.6]get point count
---
--- @return int
--- @nodiscard
function GlyphOutline:getPointCount() end
---
---[4.6]get points
---
--- @param arg0 FloatArray#floatarray
--- @param arg1 int
--- @nodiscard
function GlyphOutline:getPoints(arg0, arg1) end
---
---[4.6]get tags
---
--- @param arg0 ByteArray#bytearray
--- @param arg1 int
--- @nodiscard
function GlyphOutline:getTags(arg0, arg1) end
---
---[4.6]get contour count
---
--- @return int
--- @nodiscard
function GlyphOutline:getContourCount() end
---
---[4.6]get contours
---
--- @param arg0 IntArray#intarray
--- @param arg1 int
--- @nodiscard
function GlyphOutline:getContours(arg0, arg1) end
---
---[4.6]get flags
---
--- @return int
--- @nodiscard
function GlyphOutline:getFlags() end
---
---[4.6]and op
---
--- @param arg0 int
--- @param arg1 int
--- @return bool
--- @nodiscard
function GlyphOutline:bitAnd(arg0, arg1) end

return GlyphOutline
