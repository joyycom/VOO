--* This Document is AutoGenerate by OrangeFilter, Don't Change it! *

---@meta

---
---[3.2]material[parent:]
---
---@class MaterialLegacy
MaterialLegacy = {}

---
---[3.2]is material has valid data
---
--- @param arg0 int
--- @return bool
--- @nodiscard
function MaterialLegacy:isMaterialValid(arg0) end
---
---[3.2]get program
---
--- @param arg0 int
--- @return Program#program
--- @nodiscard
function MaterialLegacy:getProgram(arg0) end
---
---[3.2]get program pass name
---
--- @param arg0 int
--- @return string
--- @nodiscard
function MaterialLegacy:getProgramName(arg0) end
---
---[3.2]replace material shader pass
---
--- @param arg0 int
--- @param arg1 string
--- @nodiscard
function MaterialLegacy:replacePass(arg0, arg1) end
---
---[3.2]replace material shader pass
---
--- @param arg0 int
--- @param arg1 Program#program
--- @nodiscard
function MaterialLegacy:replaceProgram(arg0, arg1) end
---
---[3.2]get render queue
---
--- @param arg0 int
--- @return int
--- @nodiscard
function MaterialLegacy:getQueue(arg0) end
---
---[3.2]set float shader property
---
--- @param arg0 string
--- @param arg1 float
--- @nodiscard
function MaterialLegacy:setFloat(arg0, arg1) end
---
---[3.2]set vec4 shader property
---
--- @param arg0 string
--- @param arg1 Vec4f#vec4f
--- @nodiscard
function MaterialLegacy:setVector(arg0, arg1) end
---
---[3.2]set color shader property
---
--- @param arg0 string
--- @param arg1 Color#color
--- @nodiscard
function MaterialLegacy:setColor(arg0, arg1) end
---
---[3.2]set texture shader property
---
--- @param arg0 string
--- @param arg1 Texture#texture
--- @nodiscard
function MaterialLegacy:setTexture(arg0, arg1) end
---
---[3.2]get texture shader property
---
--- @param arg0 string
--- @return Texture#texture
--- @nodiscard
function MaterialLegacy:getTexture(arg0) end
---
---[3.2]set texture shader property
---
--- @param arg0 string
--- @param arg1 OF_Texture#of_texture
--- @nodiscard
function MaterialLegacy:setTextureRef(arg0, arg1) end
---
---[3.2]set cull mode
---
--- @param arg0 string
--- @nodiscard
function MaterialLegacy:setCull(arg0) end
---
---[4.0]set cull mode
---
--- @param arg0 int
--- @param arg1 string
--- @nodiscard
function MaterialLegacy:setCullByIndex(arg0, arg1) end
---
---[3.6]set blend mode
---
--- @param arg0 string
--- @nodiscard
function MaterialLegacy:setBlend(arg0) end
---
---[3.7]set z write enable
---
--- @param arg0 string
--- @nodiscard
function MaterialLegacy:setZWrite(arg0) end
---
---[3.7]set z test mode
---
--- @param arg0 string
--- @nodiscard
function MaterialLegacy:setZTest(arg0) end

return MaterialLegacy
