--* This Document is AutoGenerate by OrangeFilter, Don't Change it! *

---@meta

---
---[3.3]trail renderer[parent:]
---
---@class TrailRenderer
TrailRenderer = {}

---
---[3.3]constructor, 1 param
---
--- @param arg0 Context#context
--- @nodiscard
function TrailRenderer:new(arg0) end

---
---[3.3]add new point, and update buffer
---
--- @param arg0 Vec3f#vec3f
--- @param arg1 float
--- @param arg2 bool
--- @nodiscard
function TrailRenderer:update(arg0, arg1, arg2) end
---
---[3.3]add new point
---
--- @param arg0 Vec3f#vec3f
--- @param arg1 float
--- @param arg2 bool
--- @nodiscard
function TrailRenderer:addPoint(arg0, arg1, arg2) end
---
---[3.3]update buffer
---
--- @param arg0 float
--- @nodiscard
function TrailRenderer:updateBuffer(arg0) end
---
---[3.3]render
---
--- @nodiscard
function TrailRenderer:render() end
---
---[3.3]set shader
---
--- @param arg0 string
--- @nodiscard
function TrailRenderer:setShader(arg0) end
---
---[3.3]set texture
---
--- @param arg0 Texture#texture
--- @param arg1 Vec4f#vec4f
--- @nodiscard
function TrailRenderer:setTexture(arg0, arg1) end
---
---[3.3]set texture mode
---
--- @param arg0 OF_LineTextureMode#of_linetexturemode
--- @nodiscard
function TrailRenderer:setTextureMode(arg0) end
---
---[3.3]set texture tiling rate
---
--- @param arg0 float
--- @nodiscard
function TrailRenderer:setTextureTilingRate(arg0) end
---
---[3.3]get material
---
--- @return MaterialLegacy#materiallegacy
--- @nodiscard
function TrailRenderer:getMaterial() end
---
---[3.3]set min vertex distance
---
--- @param arg0 float
--- @nodiscard
function TrailRenderer:setMinVertexDistance(arg0) end
---
---[3.3]set width curve
---
--- @param arg0 AnimationCurve#animationcurve
--- @nodiscard
function TrailRenderer:setWidthCurve(arg0) end
---
---[3.3]set width multiplier
---
--- @param arg0 float
--- @nodiscard
function TrailRenderer:setWidthMultiplier(arg0) end
---
---[3.3]set color gradient
---
--- @param arg0 Gradient#gradient
--- @nodiscard
function TrailRenderer:setColorGradient(arg0) end
---
---[3.3]set trail time
---
--- @param arg0 float
--- @nodiscard
function TrailRenderer:setTime(arg0) end
---
---[3.3]set curvelize
---
--- @param arg0 bool
--- @nodiscard
function TrailRenderer:setCurvelize(arg0) end
---
---[3.3]set curve insert point count
---
--- @param arg0 int
--- @nodiscard
function TrailRenderer:setCurveInsertPointCount(arg0) end
---
---[3.3]clear all points
---
--- @nodiscard
function TrailRenderer:clear() end
---
---[3.3]get point count
---
--- @return int
--- @nodiscard
function TrailRenderer:getPositionCount() end
---
---[3.3]get point position
---
--- @param arg0 int
--- @return Vec3f#vec3f
--- @nodiscard
function TrailRenderer:getPosition(arg0) end

return TrailRenderer
