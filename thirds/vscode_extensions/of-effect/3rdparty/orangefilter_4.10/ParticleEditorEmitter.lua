--* This Document is AutoGenerate by OrangeFilter, Don't Change it! *

---@meta

---
---[4.9]particle system[parent:]
---
---@class ParticleEditorEmitter
ParticleEditorEmitter = {}

---
---[4.9]emit
---
--- @param arg0 OFUInt32
--- @nodiscard
function ParticleEditorEmitter:emitParticle(arg0) end
---
---[4.9]the emitter mode
---
--- @return ParticleEditor2D::ParticleEmitter::Mode
--- @nodiscard
function ParticleEditorEmitter:getEmitterMode() end
---
---[4.9]the emitter mode
---
--- @param arg0 ParticleEditor2D::ParticleEmitter::Mode
--- @nodiscard
function ParticleEditorEmitter:setEmitterMode(arg0) end
---
---[4.10]generate particle rand seed
---
--- @param arg0 int
--- @nodiscard
function ParticleEditorEmitter:setRandomSeed(arg0) end
---
---[4.9]the gravity
---
--- @return Vec2f#vec2f
--- @nodiscard
function ParticleEditorEmitter:getGravity() end
---
---[4.9]the gravity
---
--- @param arg0 Vec2f#vec2f
--- @nodiscard
function ParticleEditorEmitter:setGravity(arg0) end
---
---[4.9]the emitter speed
---
--- @return float
--- @nodiscard
function ParticleEditorEmitter:getSpeed() end
---
---[4.9]the emitter speed
---
--- @param arg0 float
--- @nodiscard
function ParticleEditorEmitter:setSpeed(arg0) end
---
---[4.9]the emitter speed variance
---
--- @return float
--- @nodiscard
function ParticleEditorEmitter:getSpeedVar() end
---
---[4.9]the emitter speed variance
---
--- @param arg0 float
--- @nodiscard
function ParticleEditorEmitter:setSpeedVar(arg0) end
---
---[4.9]the tangential acceleration
---
--- @return float
--- @nodiscard
function ParticleEditorEmitter:getTangentialAccel() end
---
---[4.9]the tangential acceleration
---
--- @param arg0 float
--- @nodiscard
function ParticleEditorEmitter:setTangentialAccel(arg0) end
---
---[4.9]the tangential acceleration variance
---
--- @return float
--- @nodiscard
function ParticleEditorEmitter:getTangentialAccelVar() end
---
---[4.9]the tangential acceleration variance
---
--- @param arg0 float
--- @nodiscard
function ParticleEditorEmitter:setTangentialAccelVar(arg0) end
---
---[4.9]the radial acceleration
---
--- @return float
--- @nodiscard
function ParticleEditorEmitter:getRadialAccel() end
---
---[4.9]the radial acceleration
---
--- @param arg0 float
--- @nodiscard
function ParticleEditorEmitter:setRadialAccel(arg0) end
---
---[4.9]the radial acceleration variance
---
--- @return float
--- @nodiscard
function ParticleEditorEmitter:getRadialAccelVar() end
---
---[4.9]the radial acceleration variance
---
--- @param arg0 float
--- @nodiscard
function ParticleEditorEmitter:setRadialAccelVar(arg0) end
---
---[4.9]true if the rotation is the direction
---
--- @return bool
--- @nodiscard
function ParticleEditorEmitter:getRotationIsDir() end
---
---[4.9]true if the rotation is the direction
---
--- @param arg0 float
--- @nodiscard
function ParticleEditorEmitter:setRotationIsDir(arg0) end
---
---[4.9]the start radius
---
--- @return Vec2f#vec2f
--- @nodiscard
function ParticleEditorEmitter:getStartRadius() end
---
---[4.9]the start radius
---
--- @param arg0 Vec2f#vec2f
--- @nodiscard
function ParticleEditorEmitter:setStartRadius(arg0) end
---
---[4.9]the start radius variance
---
--- @return Vec2f#vec2f
--- @nodiscard
function ParticleEditorEmitter:getStartRadiusVar() end
---
---[4.9]the start radius variance
---
--- @param arg0 Vec2f#vec2f
--- @nodiscard
function ParticleEditorEmitter:setStartRadiusVar(arg0) end
---
---[4.9]the end radius
---
--- @return Vec2f#vec2f
--- @nodiscard
function ParticleEditorEmitter:getEndRadius() end
---
---[4.9]the end radius
---
--- @param arg0 Vec2f#vec2f
--- @nodiscard
function ParticleEditorEmitter:setEndRadius(arg0) end
---
---[4.9]the end radius variance
---
--- @return Vec2f#vec2f
--- @nodiscard
function ParticleEditorEmitter:getEndRadiusVar() end
---
---[4.9]the end radius variance
---
--- @param arg0 Vec2f#vec2f
--- @nodiscard
function ParticleEditorEmitter:setEndRadiusVar(arg0) end
---
---[4.9]the number of degrees to rotate a particle per second
---
--- @return float
--- @nodiscard
function ParticleEditorEmitter:getRotatePerSecond() end
---
---[4.9]the number of degrees to rotate a particle per second
---
--- @param arg0 float
--- @nodiscard
function ParticleEditorEmitter:setRotatePerSecond(arg0) end
---
---[4.9]the number of degrees variance to rotate a particle per second
---
--- @return float
--- @nodiscard
function ParticleEditorEmitter:getRotatePerSecondVar() end
---
---[4.9]the number of degrees variance to rotate a particle per second
---
--- @param arg0 float
--- @nodiscard
function ParticleEditorEmitter:setRotatePerSecondVar(arg0) end
---
---[4.9]the source position of the emitter
---
--- @return Vec2f#vec2f
--- @nodiscard
function ParticleEditorEmitter:getSourcePosition() end
---
---[4.9]the source position of the emitter
---
--- @param arg0 Vec2f#vec2f
--- @nodiscard
function ParticleEditorEmitter:setSourcePosition(arg0) end
---
---[4.9]the position variance of the emitter
---
--- @return Vec2f#vec2f
--- @nodiscard
function ParticleEditorEmitter:getPosVar() end
---
---[4.9]the position variance of the emitter
---
--- @param arg0 [Vec2f#vec2f
--- @nodiscard
function ParticleEditorEmitter:setPosVar(arg0) end
---
---[4.9]the life of each particle
---
--- @return float
--- @nodiscard
function ParticleEditorEmitter:getLife() end
---
---[4.9]the life of each particle
---
--- @param arg0 float
--- @nodiscard
function ParticleEditorEmitter:setLife(arg0) end
---
---[4.9]the life variance of each particle
---
--- @return float
--- @nodiscard
function ParticleEditorEmitter:getLifeVar() end
---
---[4.9]the life variance of each particle
---
--- @param arg0 float
--- @nodiscard
function ParticleEditorEmitter:setLifeVar(arg0) end
---
---[4.9]the angle of each particle of each particle
---
--- @return float
--- @nodiscard
function ParticleEditorEmitter:getAngle() end
---
---[4.9]the angle of each particle of each particle
---
--- @param arg0 float
--- @nodiscard
function ParticleEditorEmitter:setAngle(arg0) end
---
---[4.9]the angle variance of each particle of each particle
---
--- @return float
--- @nodiscard
function ParticleEditorEmitter:getAngleVar() end
---
---[4.9]the angle variance of each particle of each particle
---
--- @param arg0 float
--- @nodiscard
function ParticleEditorEmitter:setAngleVar(arg0) end
---
---[4.9]the start size in pixels
---
--- @return float
--- @nodiscard
function ParticleEditorEmitter:getStartSize() end
---
---[4.9]the start size in pixels
---
--- @param arg0 float
--- @nodiscard
function ParticleEditorEmitter:setStartSize(arg0) end
---
---[4.9]the start size in pixels
---
--- @param arg0 ByteArray#bytearray
--- @param arg1 int
--- @nodiscard
function ParticleEditorEmitter:setTweenSize(arg0, arg1) end
---
---[4.9]the start size variance in pixels
---
--- @return float
--- @nodiscard
function ParticleEditorEmitter:getStartSizeVar() end
---
---[4.9]the start size variance in pixels
---
--- @param arg0 float
--- @nodiscard
function ParticleEditorEmitter:setStartSizeVar(arg0) end
---
---[4.10]the tween color rgb
---
--- @param arg0 ByteArray#bytearray
--- @param arg1 ByteArray#bytearray
--- @param arg2 ByteArray#bytearray
--- @param arg3 int
--- @nodiscard
function ParticleEditorEmitter:setTweenColorRGB(arg0, arg1, arg2, arg3) end
---
---[4.10]the tween color alpha
---
--- @param arg0 ByteArray#bytearray
--- @param arg1 int
--- @nodiscard
function ParticleEditorEmitter:setTweenColorAlpha(arg0, arg1) end
---
---[4.10]the tween color mode
---
--- @param arg0 ParticleEditor2D::ParticleEmitter::ColorMode
--- @nodiscard
function ParticleEditorEmitter:setColorMode(arg0) end
---
---[4.9]the color variance of each particle
---
--- @param arg0 Vec4us#vec4us
--- @param arg1 Vec4us#vec4us
--- @nodiscard
function ParticleEditorEmitter:setColorVar(arg0, arg1) end
---
---[4.9]the start size in pixels
---
--- @return float
--- @nodiscard
function ParticleEditorEmitter:getStartSpin() end
---
---[4.9]the start size in pixels
---
--- @param arg0 float
--- @nodiscard
function ParticleEditorEmitter:setStartSpin(arg0) end
---
---[4.9]the start size variance in pixels
---
--- @return float
--- @nodiscard
function ParticleEditorEmitter:getStartSpinVar() end
---
---[4.9]the start size variance in pixels
---
--- @param arg0 float
--- @nodiscard
function ParticleEditorEmitter:setStartSpinVar(arg0) end
---
---[4.9]the end spin of each particle
---
--- @return float
--- @nodiscard
function ParticleEditorEmitter:getEndSpin() end
---
---[4.9]the end spin of each particle
---
--- @param arg0 float
--- @nodiscard
function ParticleEditorEmitter:setEndSpin(arg0) end
---
---[4.9]the end spin variance of each particle
---
--- @return float
--- @nodiscard
function ParticleEditorEmitter:getEndSpinVar() end
---
---[4.9]the end spin variance of each particle
---
--- @param arg0 float
--- @nodiscard
function ParticleEditorEmitter:setEndSpinVar(arg0) end
---
---[4.9]the emission rate of the particles
---
--- @return float
--- @nodiscard
function ParticleEditorEmitter:getEmissionRate() end
---
---[4.9]the emission rate of the particles
---
--- @param arg0 float
--- @nodiscard
function ParticleEditorEmitter:setEmissionRate(arg0) end

return ParticleEditorEmitter
