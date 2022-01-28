TAG = "OF-Sharp"
OF_LOGI(TAG, "Call Sharp lua script!")

local Filter = {
	imageTexMax = nil,
	renderPass = nil,
	intensity = 0,
	vs = [[
		precision highp float;
        attribute vec4 aPosition;
        attribute vec4 aTextureCoord;
		uniform float uImageWidth;
		uniform float uImageHeight;
        varying vec2 vTexCoord;
		varying vec2 vRenderSize;

        void main()
        {
            gl_Position = aPosition;
            vTexCoord = aTextureCoord.xy;
			vRenderSize = vec2(uImageWidth, uImageHeight);
        }
        ]],

    fs = [[
        precision highp float;
		uniform sampler2D uTexture0;
		uniform float uIntensity;
		varying vec2 vTexCoord;
		varying vec2 vRenderSize;

        void main()
        {
			float surfaceWidth = vRenderSize.x;
			float surfaceHeight = vRenderSize.y;

			vec2 uv = vTexCoord;
			vec4 color = texture2D(uTexture0, uv);
			vec4 result_color = color;

			float bitmapMaxLength = max(surfaceHeight, surfaceWidth);
			float f = abs(uIntensity);
			float f2 = (bitmapMaxLength - 1000.0) / 2000.0;
			f2 = max(0.0, min(f2, 1.0));
			f = ((f * 4.0) * (((1.0 - f2) * 0.65) + (f2 * 1.2))) + 1.0;
			float f3 = (1.0 - f) * 0.25;

			vec4 color_left = texture2D(uTexture0, uv+vec2(-1.0/surfaceWidth, 0.0));
			vec4 color_right = texture2D(uTexture0, uv+vec2(1.0/surfaceWidth, 0.0));
			vec4 color_bottom = texture2D(uTexture0, uv+vec2(0.0, 1.0/surfaceHeight));
			vec4 color_top = texture2D(uTexture0, uv+vec2(0.0, -1.0/surfaceHeight));
			result_color.rgb = f*result_color.rgb + f3*color_left.rgb + f3*color_right.rgb + f3*color_top.rgb + f3*color_bottom.rgb;

			gl_FragColor = result_color;
        }
        ]],
}

function Filter:initParams(context, filter)
	OF_LOGI(TAG, "call initParams")
	filter:insertFloatParam("Intensity", 0, 1, 0)
	return OF_Result_Success
end

function Filter:onApplyParams(context, filter)
	OF_LOGI(TAG, "call onApplyParams")
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
	self.renderPass:setUniform1f("uImageWidth", width)
	self.renderPass:setUniform1f("uImageHeight", height)
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
