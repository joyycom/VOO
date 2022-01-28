--* This Document is AutoGenerate by OrangeFilter, Don't Change it! *

---@meta

---
---[3.2]shader program[parent:]
---
---@class Program
Program = {}

---
---[3.2]init program by shader source
---
--- @param arg0 string
--- @param arg1 string
--- @param arg2 bool
--- @return bool
--- @nodiscard
function Program:init(arg0, arg1, arg2) end
---
---[3.2]init program by shader source file
---
--- @param arg0 string
--- @param arg1 string
--- @return bool
--- @nodiscard
function Program:initFromFile(arg0, arg1) end
---
---[3.2]use program
---
--- @nodiscard
function Program:use() end
---
---[3.2]get attribute or uniform location
---
--- @param arg0 string
--- @param arg1 bool
--- @return int
--- @nodiscard
function Program:getHandle(arg0, arg1) end
---
---[3.2]set uniform 1 int
---
--- @param arg0 string
--- @param arg1 int
--- @nodiscard
function Program:setUniform1i(arg0, arg1) end
---
---[3.2]set uniform ivec2
---
--- @param arg0 string
--- @param arg1 int
--- @param arg2 int
--- @nodiscard
function Program:setUniform2i(arg0, arg1, arg2) end
---
---[3.2]set uniform 1 float
---
--- @param arg0 string
--- @param arg1 float
--- @nodiscard
function Program:setUniform1f(arg0, arg1) end
---
---[3.2]set uniform vec2
---
--- @param arg0 string
--- @param arg1 float
--- @param arg2 float
--- @nodiscard
function Program:setUniform2f(arg0, arg1, arg2) end
---
---[3.2]set uniform vec3
---
--- @param arg0 string
--- @param arg1 float
--- @param arg2 float
--- @param arg3 float
--- @nodiscard
function Program:setUniform3f(arg0, arg1, arg2, arg3) end
---
---[3.2]set uniform vec4
---
--- @param arg0 string
--- @param arg1 float
--- @param arg2 float
--- @param arg3 float
--- @param arg4 float
--- @nodiscard
function Program:setUniform4f(arg0, arg1, arg2, arg3, arg4) end
---
---[3.2]set uniform vec4 or vec4 array
---
--- @param arg0 string
--- @param arg1 int
--- @param arg2 float[]
--- @nodiscard
function Program:setUniform4fv(arg0, arg1, arg2) end
---
---[3.2]set uniform mat3 or mat3 array
---
--- @param arg0 string
--- @param arg1 int
--- @param arg2 bool
--- @param arg3 float[]
--- @nodiscard
function Program:setUniformMatrix3fv(arg0, arg1, arg2, arg3) end
---
---[3.2]set uniform mat4 or mat4 array
---
--- @param arg0 string
--- @param arg1 int
--- @param arg2 bool
--- @param arg3 float[]
--- @nodiscard
function Program:setUniformMatrix4fv(arg0, arg1, arg2, arg3) end
---
---[3.2]set uniform texture
---
--- @param arg0 string
--- @param arg1 int
--- @param arg2 int
--- @param arg3 int
--- @nodiscard
function Program:setUniformTexture(arg0, arg1, arg2, arg3) end
---
---[3.7]set texture
---
--- @param arg0 string
--- @param arg1 int
--- @param arg2 OF_Texture#of_texture
--- @nodiscard
function Program:setTexture(arg0, arg1, arg2) end
---
---[3.3]set vertex attribute
---
--- @param arg0 string
--- @param arg1 int
--- @param arg2 GL_DATA_TYPE#gl_data_type
--- @param arg3 bool
--- @param arg4 int
--- @param arg5 int
--- @nodiscard
function Program:setVertexAttrib(arg0, arg1, arg2, arg3, arg4, arg5) end
---
---[3.3]disable vertex attribute
---
--- @param arg0 string
--- @nodiscard
function Program:disableVertexAttrib(arg0) end
---
---[3.3]set vertex attribute divisor,for draw instancing
---
--- @param arg0 string
--- @param arg1 int
--- @nodiscard
function Program:setVertexAttribDivisor(arg0, arg1) end
---
---[3.3]draw with vertex buffer and index buffer
---
--- @param arg0 GL_PRIMITIVE_MODE#gl_primitive_mode
--- @param arg1 int
--- @param arg2 GL_DATA_TYPE#gl_data_type
--- @param arg3 int
--- @nodiscard
function Program:drawElements(arg0, arg1, arg2, arg3) end
---
---[4.5]draw with vertex buffer and index buffer
---
--- @param arg0 GL_PRIMITIVE_MODE#gl_primitive_mode
--- @param arg1 int
--- @param arg2 GL_DATA_TYPE#gl_data_type
--- @param arg3 int
--- @param arg4 int
--- @nodiscard
function Program:drawElementsInstanced(arg0, arg1, arg2, arg3, arg4) end
---
---[3.3]draw with vertex buffer
---
--- @param arg0 GL_PRIMITIVE_MODE#gl_primitive_mode
--- @param arg1 int
--- @param arg2 int
--- @nodiscard
function Program:drawArrays(arg0, arg1, arg2) end
---
---[4.5]draw with vertex buffer
---
--- @param arg0 GL_PRIMITIVE_MODE#gl_primitive_mode
--- @param arg1 int
--- @param arg2 int
--- @param arg3 int
--- @nodiscard
function Program:drawArraysInstanced(arg0, arg1, arg2, arg3) end

return Program
