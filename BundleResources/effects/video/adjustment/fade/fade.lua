TAG = "OF-Fade"
OF_LOGI(TAG, "Call Fade lua script!")

local Filter = {
	imageTexMax = nil,
	renderPass = nil,
	intensity = 0,
	vs = [[
		precision highp float;
        attribute vec4 aPosition;
        attribute vec4 aTextureCoord;
        varying vec2 vTexCoord;

        void main()
        {
            gl_Position = aPosition;
            vTexCoord = aTextureCoord.xy;
        }
        ]],

    fs = [[
        precision highp float;
		uniform sampler2D uTexture0;
		uniform sampler2D uTexture1;
		uniform float uIntensity;
		varying vec2 vTexCoord;

        void main()
        {
			float slider_progress = abs(uIntensity);
			slider_progress = slider_progress * 0.7;
			vec4 curColor = texture2D(uTexture0, vTexCoord);
			vec4 textureColor = curColor;
			float blueColor = curColor.b * (17.0 - 1.0);
			vec2 standardTableSize = vec2(289.0, 17.0);
			vec2 pixelSize = 1.0 / standardTableSize;
			vec2 quad1 = vec2(0.0);
			quad1.y = floor(floor(blueColor) / 17.0);
			quad1.x = floor(blueColor) - (quad1.y * 1.0);
			vec2 quad2;
			quad2.y = floor(ceil(blueColor) / 17.0);
			quad2.x = ceil(blueColor) - (quad2.y * 1.0);

			vec2 texPos1;
			texPos1.x = (quad1.x * 1.0 / 17.0) + 0.5 / standardTableSize.x + ((1.0 / 17.0 - 1.0 / standardTableSize.x) * textureColor.r);
			texPos1.y = (quad1.y * 1.0 / 1.0) + 0.5 / standardTableSize.y +((1.0 / 1.0 - 1.0 / standardTableSize.y) * textureColor.g);

			vec2 texPos2;
			texPos2.x = (quad2.x * 1.0 / 17.0) + 0.5 / standardTableSize.x + ((1.0 / 17.0 - 1.0 / standardTableSize.x) * textureColor.r);
			texPos2.y = (quad2.y * 1.0 / 1.0) + 0.5 / standardTableSize.y +((1.0 / 1.0 - 1.0 / standardTableSize.y) * textureColor.g);

			float alpha = fract(blueColor);
			vec4 newColor1 = texture2D(uTexture1, texPos1);
			vec4 newColor2 = texture2D(uTexture1, texPos2);
			vec4 newColor = mix(newColor1, newColor2, alpha);
			gl_FragColor = mix(curColor,newColor,slider_progress);
        }
        ]],
}

function Filter:initParams(context, filter)
	OF_LOGI(TAG, "call initParams")
	filter:insertFloatParam("Intensity", 0, 1, 0)
    filter:insertResParam("ImageMax", OF_ResType_Image, "")
	return OF_Result_Success
end

function Filter:onApplyParams(context, filter)
	OF_LOGI(TAG, "call onApplyParams")
	local path = filter:resParam("ImageMax")
	local fullpath = filter:resFullPath(path)
	if self.imageTexMax then context:destroyTexture(self.imageTexMax) end -- destroy if exist
	self.imageTexMax = context:loadTextureFromFile(fullpath, TEXTURE_2D, LINEAR, CLAMP_TO_EDGE, false, false)

	if self.intensity then
		self.intensity = filter:floatParam("Intensity")
	end
	return OF_Result_Success
end

function Filter:initRenderer(context, filter)
	OF_LOGI(TAG, "call initRenderer")
	self.renderPass = context:createCustomShaderPass(self.vs, self.fs)
	return OF_Result_Success
end

function Filter:teardownRenderer(context, filter)
	OF_LOGI(TAG, "call teardownRenderer")
	context:destroyCustomShaderPass(self.renderPass)
	if self.imageTexMax then context:destroyTexture(self.imageTexMax) end -- destroy if exist
	return OF_Result_Success
end

function Filter:applyRGBA(context, filter, frameData, inTex, outTex, debugTex)
	local width = outTex.width
	local height = outTex.height
	context:setViewport(0, 0, width, height)
	context:bindFBO(outTex)
	context:setBlend(false)

	self.renderPass:use()
	self.renderPass:setUniformTexture("uTexture0", 0, inTex.textureID, TEXTURE_2D)
	self.renderPass:setUniformTexture("uTexture1", 1, self.imageTexMax:textureID(), TEXTURE_2D)
	self.renderPass:setUniform1f("uIntensity", self.intensity)

	local quadRender = context:sharedQuadRender()
	quadRender:draw(self.renderPass, false)

	if debugTex ~= nil then
		context:copyTexture(inTex, debugTex)
	end
	return OF_Result_Success
end

function Filter:requiredFrameData(context, game)
	return { OF_RequiredFrameData_None }
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
	local tab = Json.JsonToTable(msg)
	if tab.intensity then
		self.intensity = tab.intensity
	end
	return ""
end

return Filter
