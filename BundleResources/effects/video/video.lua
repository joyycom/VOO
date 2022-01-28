-- Change history:
--   2021/04/20 : Optimize.
--   2021/09/17 : add renderToRT in animation.
--   2021/09/27 : fix video texture reuse bug

TAG = "OrangeFilter-Video"
OF_LOGI(TAG, "Call Video lua script!")
local EffectList = require "sub.effectlist"
local Animation = require "sub.animation"
local MaskRender = require "common.mask"
local TextureRender = require "common.texture"
local BlendRender = require "common.blend"

local Filter = {
    name = "video",
    
	imageTex = nil,
	maskTex = nil,
	duration = 10,
	context = nil,
	params = {
		tx = 0, ty = 0, rot = 0, scale = 1,
		scaleX = 1,
		scaleY = 1,
		fourCornerMat = Matrix4f.new(),
		rotX = 0, rotY = 0,
		opacity = 100,
		},
	fSetParamsByMsg = false,

	animation = Animation,
	
	timestamp = 0,
	debug = 0.0
}

function Filter:initParams(context, filter)
	OF_LOGI(TAG, "call initParams")
	filter:insertBoolParam("Camera", true)

	filter:insertFloatParam("TransX", -10000, 10000, 0)
	filter:insertFloatParam("TransY", -10000, 10000, 0)
	filter:insertFloatParam("Rotate", -180, 180, 0)
	filter:insertFloatParam("RotateX", -180, 180, 0)
	filter:insertFloatParam("RotateY", -180, 180, 0)
	filter:insertFloatParam("Scale", 0.001, 10, 1.0)
	filter:insertFloatParam("ScaleX", 0.001, 10, 1.0)
    filter:insertFloatParam("ScaleY", 0.001, 10, 1.0)

	if filter.insertFloatArrayParam then  -- version compatible
		filter:insertFloatArrayParam("FourCornerMat", Matrix4f.new().x)
	end
    filter:insertIntParam("Opacity", 0, 100, 100)

	TextureRender:initParams(context, filter)
	BlendRender:initParams(context, filter)
	MaskRender:initParams(context, filter)
	
	EffectList:initParams(context, filter)
	Animation:initParams(context, filter)

	filter:insertFloatParam("debug", 0, 0.2, 0)
	return OF_Result_Success
end

function Filter:onApplyParams(context, filter)
	OF_LOGI(TAG, "call onApplyParams")
	if self.fSetParamsByMsg == false then
		self.params.tx = filter:floatParam("TransX")
		self.params.ty = filter:floatParam("TransY")
		self.params.rot = filter:floatParam("Rotate") * math.pi / 180
		self.params.rotX = filter:floatParam("RotateX") * math.pi / 180
		self.params.rotY = filter:floatParam("RotateY") * math.pi / 180
		self.params.scale = filter:floatParam("Scale")
		self.params.scaleX = filter:floatParam("ScaleX")
		self.params.scaleY = filter:floatParam("ScaleY")
	end
	
	if filter.floatArrayParam then -- version compatible
		self.params.fourCornerMat = Matrix4f.new(filter:floatArrayParam("FourCornerMat"))
	end
	self.debug = filter:floatParam("debug")
	
	self.params.opacity = filter:intParam("Opacity")

	--Animation
	Animation:loadFromFilter(context, filter)

	BlendRender:onApplyParams(context, filter)
	MaskRender:onApplyParams(context, filter)
	TextureRender:onApplyParams(context, filter)
	
	EffectList:onApplyParams(context, filter)

	return OF_Result_Success
end

function Filter:initRenderer(context, filter)
	OF_LOGI(TAG, "call initRenderer")
	self.context = context

	TextureRender:initRenderer(context, filter)
	BlendRender:initRenderer(context, filter)
	MaskRender:initRenderer(context, filter)

	return OF_Result_Success
end

function Filter:teardownRenderer(context, filter)
	OF_LOGI(TAG, "call teardownRenderer")

	TextureRender:teardown(context)
	BlendRender:teardown(context)
	MaskRender:teardown(context)

	if self.imageTex then 
		context:destroyTexture(self.imageTex) 
		self.imageTex = nil
	end -- destroy if exist
	
	EffectList:clear(context)

	Animation:clear(context)

	return OF_Result_Success
end

function Filter:drawFrame(context, baseTex, outTex, mvpMat)
	context:bindFBO(outTex)
	context:setViewport(0, 0, outTex.width, outTex.height)   

	if baseTex ~= nil then 
		context:copyTexture(baseTex, outTex)
		context:setBlend(true)
		context:setBlendMode(RS_BlendFunc_SRC_ALPHA, RS_BlendFunc_INV_SRC_ALPHA)
	else
		context:setBlend(false)
		context:setClearColor(0.0, 0.0, 0.0, 0.0)
		context:clearColorBuffer()
	end

	TextureRender:setOpacity(Animation.params.alpha * self.params.opacity / 100)
	TextureRender:draw(context, self.imageTex:toOFTexture(), outTex, mvpMat)

	context:setBlend(false)

	MaskRender:draw(context, nil, self.maskTex, outTex, Matrix4f:ScaleMat(1.0, 1.0, 1.0))
end

function Filter:renderToOutputDirectly(context, filter, baseTex, outTex)
	local width, height = outTex.width, outTex.height

	local scaleMat = Matrix4f:ScaleMat(self.params.scaleX * self.params.scale, self.params.scaleY * self.params.scale, 1.0)
	local rotMat = Matrix4f:RotMat(0, 0, self.params.rot)
	local transMat = Matrix4f:TransMat(self.params.tx, self.params.ty, 0.0)

	local animLocalTransMat = Matrix4f:TransMat(
		Animation.params.localPosition[1], Animation.params.localPosition[2],
		Animation.params.localPosition[3])
	local animLocalScaleMat = Matrix4f:ScaleMat(
		Animation.params.localScale[1], Animation.params.localScale[2],
		Animation.params.localScale[3])
	local animLocalRotMat = Matrix4f:RotMat(
		Animation.params.localRotation[1], Animation.params.localRotation[2],
		Animation.params.localRotation[3])

	local animTransMat = Matrix4f:TransMat(
		Animation.params.position[1], Animation.params.position[2], Animation.params.position[3])
	local animScaleMat = Matrix4f:ScaleMat(
		Animation.params.scale[1], Animation.params.scale[2],	Animation.params.scale[3])
		
	local animRotMat = Matrix4f:RotMat(
		Animation.params.rotation[1], Animation.params.rotation[2], Animation.params.rotation[3])
	
	local viewMat, projMat = Matrix4f.new(), Matrix4f.new()
	local rotXMat, rotYMat = Matrix4f.new(), Matrix4f.new()
	if filter:boolParam("Camera") then
		viewMat = Matrix4f:LookAtMat(Vec3f.new(0, 0, 8.0), Vec3f.new(0, 0, 0.0), Vec3f.new(0.0, 1.0, 0.0))
		projMat = Matrix4f:PerspectiveMat(14.25, 1.0, 1.0, 101.0) --arctan(0.125) * 2 = 14.25
		rotXMat, rotYMat = Matrix4f:RotMat(self.params.rotX, 0, 0), Matrix4f:RotMat(0, self.params.rotY, 0)
	end

	local mvpMat = 
		Matrix4f:ScaleMat(2 / width, 2 / height, 1.0 ) * self.params.fourCornerMat * 
		animTransMat * transMat * projMat * viewMat *
		animRotMat * rotMat *
		animScaleMat * scaleMat *
		animLocalTransMat * animLocalRotMat * animLocalScaleMat *
		Matrix4f:ScaleMat(self.imageTex:width() * 0.5, self.imageTex:height() * 0.5, 1)
		* rotXMat * rotYMat
		
	local blendTex = context:getTexture(width, height)

	self.drawFrame(self, context, nil, blendTex:toOFTexture(), mvpMat)

	BlendRender:draw(context, baseTex, blendTex:toOFTexture(), outTex, Matrix4f:ScaleMat(1.0, 1.0 , 1.0))

	context:releaseTexture(blendTex)
end

function Filter:renderThroughRT(context, filter, baseTex, outTex)
	local width, height = outTex.width, outTex.height

	local texTemp = context:getTexture(width, height)

	Animation:apply(self, filter, texTemp:toOFTexture())
	
	BlendRender:draw(context, baseTex, texTemp:toOFTexture(), outTex, Matrix4f:ScaleMat(1.0, 1.0 , 1.0))

	context:releaseTexture(texTemp)
end

function Filter:applyFrame(context, filter, frameData, inTexArray, outTexArray)	
	if inTexArray[2] == nil then
        context:copyTexture(inTexArray[1], outTexArray[1])
        return OF_Result_Success
    end

	local imageTexOF = inTexArray[2]
	--
	-- apply extra effects to inTexArray[2]
	--
	local tempTex = context:getTexture(imageTexOF.width, imageTexOF.height)
	if not EffectList:isEmpty() then
		EffectList:apply(context, frameData, inTexArray[2], tempTex:toOFTexture(),
			outTexArray[2], filter:filterTimestamp())
		imageTexOF = tempTex:toOFTexture()
	end

	if self.imageTex == nil then
		self.imageTex = context:createTextureRef(imageTexOF)
	else
		context:updateTextureRef(self.imageTex, imageTexOF)
	end
	--
	-- apply maskTex from inTexArray[3]
	--
	self.maskTex =  inTexArray[3]

	Animation:seek(self, filter)

	if Animation.params.renderToRT then
		self.renderThroughRT(self, context, filter, inTexArray[1], outTexArray[1])
	else
		self.renderToOutputDirectly(self, context, filter, inTexArray[1], outTexArray[1])
	end

	-- debug tex
	if outTexArray[2] ~= nil then
		context:copyTexture(inTexArray[1], outTexArray[2])
	end

	if tempTex then context:releaseTexture(tempTex) end

	return OF_Result_Success
end

function Filter:requiredFrameData(context, filter)
    if filter:intParam("ThinFace") > 0 then
        return { OF_RequiredFrameData_FaceLandmarker }
    else
		return { OF_RequiredFrameData_None }
    end
end

function Filter:readObject(context, filter, archiveIn)
	OF_LOGI(TAG, "call readObject")
	return OF_Result_Success
end

function Filter:writeObject(context, filter, archiveOut)
	OF_LOGI(TAG, "call writeObject")
	return OF_Result_Success
end

function Filter:onReceiveMessage(context, filter, msg)
	OF_LOGI(TAG, string.format("call onReceiveMessage %s", msg))
	local evt = Json.JsonToTable(msg)
	if evt.id == 1 then
		self.fSetParamsByMsg = true
		self.params.tx = evt.params[1]
		self.params.ty = evt.params[2]
		self.params.scale = evt.params[3]
		self.params.rot = evt.params[4] * math.pi / 180
	else
		Animation:loadFromMsg(context, evt)
	end
	return ""
end

return Filter
