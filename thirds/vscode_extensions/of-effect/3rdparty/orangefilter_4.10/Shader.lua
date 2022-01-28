--* This Document is AutoGenerate by OrangeFilter, Don't Change it! *

---@meta

---
---[3.7]shader[parent:Object#object]
---
---@class Shader
Shader = {}

---
---[3.7]get pass count
---
--- @return int
--- @nodiscard
function Shader:getPassCount() end
---
---[3.7]get program
---
--- @param arg0 int
--- @return Program#program
--- @nodiscard
function Shader:getProgram(arg0) end
---
---[3.7]get render queue
---
--- @return int
--- @nodiscard
function Shader:getQueue() end

---
---[4.1]find shader by name
---
--- @param arg0 string
--- @param arg1 string[]
--- @return Shader#shader
--- @nodiscard
function Shader.Find(arg0, arg1) end

return Shader
