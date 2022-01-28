--* This Document is AutoGenerate by OrangeFilter, Don't Change it! *

---@meta

---
---[3.7]material[parent:Object#object]
---
---@class Material
Material = {}

---
---[4.3]get name
---
--- @return string
--- @nodiscard
function Material:getName() end
---
---[3.7]set matrix
---
--- @param arg0 string
--- @param arg1 Matrix4f#matrix4f
--- @nodiscard
function Material:setMatrix(arg0, arg1) end
---
---[3.7]set vector
---
--- @param arg0 string
--- @param arg1 Vec4f#vec4f
--- @nodiscard
function Material:setVector(arg0, arg1) end
---
---[3.7]set color
---
--- @param arg0 string
--- @param arg1 Color#color
--- @nodiscard
function Material:setColor(arg0, arg1) end
---
---[3.7]set float
---
--- @param arg0 string
--- @param arg1 float
--- @nodiscard
function Material:setFloat(arg0, arg1) end
---
---[3.7]set int
---
--- @param arg0 string
--- @param arg1 int
--- @nodiscard
function Material:setInt(arg0, arg1) end
---
---[3.7]set texture
---
--- @param arg0 string
--- @param arg1 OF_Texture#of_texture
--- @nodiscard
function Material:setTextureOF(arg0, arg1) end
---
---[3.7]get render queue
---
--- @return int
--- @nodiscard
function Material:getQueue() end
---
---[3.7]set render queue
---
--- @param arg0 int
--- @nodiscard
function Material:setQueue(arg0) end
---
---[3.7]get shader
---
--- @return Shader#shader
--- @nodiscard
function Material:getShader() end
---
---[3.7]set shader
---
--- @param arg0 Shader#shader
--- @nodiscard
function Material:setShader(arg0) end
---
---[4.1]enable shader keywords
---
--- @param arg0 string[]
--- @nodiscard
function Material:enableKeywords(arg0) end
---
---[4.1]enable shader keyword
---
--- @param arg0 string
--- @nodiscard
function Material:enableKeyword(arg0) end
---
---[4.1]disable shader keyword
---
--- @param arg0 string
--- @nodiscard
function Material:disableKeyword(arg0) end

return Material
