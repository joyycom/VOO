--* This Document is AutoGenerate by OrangeFilter, Don't Change it! *

---@meta

---
---[4.7]make up data[parent:]
---
---
---[4.7]name string
---
---@field name string
---
---[4.7]res type
---
---@field resType number
---
---[4.7]res file name
---
---@field resName string
---
---[4.7]texturecoords count number
---
---@field textureCoordsCount number
---
---[4.7]indices count number
---
---@field indicesCount number
---
---[4.7]indices
---
---@field indices int []
---
---[4.7]textureCoords
---
---@field textureCoords float []

---@class OF_ParamMakeupRes
OF_ParamMakeupRes = {}

---
---[4.7]constructor, no param
---
--- @return 
--- @nodiscard
function OF_ParamMakeupRes.new() end

---
---[4.7]set OF_ParamMakeupRes res name
---
--- @param arg0 OF_ParamMakeupRes#of_parammakeupres
--- @param arg1 res name
--- @return void
--- @nodiscard
function string.SetMakeupResName(arg0, arg1) end
---
---[4.7]set OF_ParamMakeupRes textureCoords and textureCoordsCount
---
--- @param arg0 OF_ParamMakeupRes#of_parammakeupres
--- @param arg1 textureCoords data
--- @param arg2 textureCoords count
--- @return void
--- @nodiscard
function string.SetTextureCoords(arg0, arg1, arg2) end
---
---[4.7]set OF_ParamMakeupRes Indices and Indices Count
---
--- @param arg0 OF_ParamMakeupRes#of_parammakeupres
--- @param arg1 Indices data
--- @param arg2 Indices count
--- @return void
--- @nodiscard
function string.SetIndices(arg0, arg1, arg2) end

return OF_ParamMakeupRes
