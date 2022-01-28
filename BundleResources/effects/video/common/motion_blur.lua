local TAG = "MotionBlurRender"

local MotionBlurRender = {
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
    
    blur_fs = [[
        precision highp float;
    
        varying vec2 vTexCoord;

        uniform sampler2D uTex0;
        uniform sampler2D uTex1;

        uniform float uBlurStrength;
    
        void main()
        {
            vec4 lastColor = texture2D(uTex0, vTexCoord);
            vec4 curColor = texture2D(uTex1, vTexCoord);
            gl_FragColor = vec4(mix(curColor.rgb, lastColor.rgb, uBlurStrength), curColor.a);
        }
    ]],

    _blurPass = nil,
    _blurStrength = 0.0,
    _lastTex = nil
}

function MotionBlurRender:initRenderer(context, filter)
    OF_LOGI("MotionBlurRender", "MotionBlurRender:initRenderer")

    self._blurPass = context:createCustomShaderPass(self.vs, self.blur_fs)

    return OF_Result_Success
end

function MotionBlurRender:teardown(context, filter)
    OF_LOGI("MotionBlurRender", "MotionBlurRender:teardown")

    context:destroyCustomShaderPass(self._blurPass)
    self._blurPass = nil
	
    if self._lastTex ~= nil then 
        context:releaseTexture(self._lastTex) 
        self._lastTex = nil
    end

    return OF_Result_Success
end

function MotionBlurRender:initParams(context, filter)
	--filter:insertFloatParam("Time", 0.0, 1.0, 0.4)
	-- filter:insertFloatParam("Pow", 0.0, 2.0, 1.0)
    -- filter:insertIntParam("BlurFrame", 0, 4, 1)
	filter:insertFloatParam("BlurStrength", 0.0, 0.95, 0.3)

    return OF_Result_Success
end

function MotionBlurRender:onApplyParams(context, filter, dirtyTable)
	self._blurStrength = filter:floatParam("BlurStrength")

    return OF_Result_Success
end

function MotionBlurRender:draw(context, inTex, outTex)
    local width = outTex.width
    local height = outTex.height
    
    local blurStrength = self._blurStrength
    if self._lastTex == nil then
        self._lastTex = context:getTexture(width, height)
        context:copyTexture(inTex, self._lastTex:toOFTexture())
    end

    local render = context:sharedQuadRender()

    context:bindFBO(outTex)
    context:setViewport(0, 0, width, height)
    self._blurPass:use()
    self._blurPass:setUniformTexture("uTex0", 0, self._lastTex:textureID(), GL_TEXTURE_2D)
    self._blurPass:setUniformTexture("uTex1", 1, inTex.textureID, GL_TEXTURE_2D)
    self._blurPass:setUniform1f("uBlurStrength", blurStrength)

    render:draw(self._blurPass, false)

    context:copyTexture(outTex, self._lastTex:toOFTexture())

    return OF_Result_Success
end

return MotionBlurRender
