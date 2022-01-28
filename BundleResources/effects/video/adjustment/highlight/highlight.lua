TAG = "OF-Highlight"
OF_LOGI(TAG, "Call Highlight lua script!")

local Filter = {
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
		uniform float uIntensity;
		varying vec2 vTexCoord;

        void main()
        {
			vec4 curColor = texture2D(uTexture0, vTexCoord);
			
			float lumR = 0.299;
			float lumG = 0.587;
			float lumB = 0.114;
			float luminance = sqrt(lumR*pow(curColor.r,2.0) + lumG*pow(curColor.g,2.0) + lumB*pow(curColor.b,2.0));
			float h = uIntensity * 0.05 * (pow(8.0, luminance) - 1.0);
			//float s = shadows * 0.05 * (pow(8.0, 1.0 - luminance) - 1.0);

			vec3 ss = vec3(1.0, 1.0, 1.0);
			ss.r = curColor.r + h;// + s;
			ss.g = curColor.g + h;// + s;
			ss.b = curColor.b + h;// + s;
			gl_FragColor = vec4(ss, 1.0);
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
