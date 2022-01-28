local TAG = "GaussBlurRender"
local gaussBlurRender = {
    vs = [[
        precision highp float;
        uniform mat4 uMVP;
        attribute vec4 aPosition;
        attribute vec4 aTextureCoord;
        attribute vec4 aColor;
        varying vec2 vTexCoord;
        void main()
        {
            gl_Position = uMVP * aPosition;
            vTexCoord = aTextureCoord.xy;
        }
        ]],
    fs_gauss_blur = [[
        precision highp float;
        varying vec2 vTexCoord;
        uniform sampler2D uTexture;
        uniform vec2 uOnePixel;
        uniform float uKernel[50];
        uniform int uKernelSize;
        
        void main()
        {
            vec4 color = texture2D(uTexture, vTexCoord) * uKernel[0];
            for (int i = 1; i < uKernelSize; i++)
            {
                vec2 pixelOffset = uOnePixel * vec2(i);
                color += texture2D(uTexture, vTexCoord + pixelOffset) * uKernel[i];
                color += texture2D(uTexture, vTexCoord - pixelOffset) * uKernel[i];
            }
            gl_FragColor = color;
        }
        ]],
    
    blurPass = nil,
    blurKernel = FloatArray.new(50),
    blurKernelSize = 3,
    blurHorizontal = true,
    blurVertical = true,
    blurIterCount = 2
}

function gaussBlurRender:initParams(context, filter)
    filter:insertFloatParam("BlurStrength", 0.0, 1.0, 0.5)
	filter:insertIntParam("BlurStep", 1, 10, self.blurIterCount)
    filter:insertEnumParam("BlurDirection", 0, { "HorizontalAndVertical", "Horizontal", "Vertical"})
end

function gaussBlurRender:initRenderer(context, filter)
    OF_LOGI(TAG, "call initRenderer")
    self.blurPass = context:createCustomShaderPass(self.vs, self.fs_gauss_blur)
    return OF_Result_Success
end

function gaussBlurRender:teardown(context)
    OF_LOGI(TAG, "call teardownRenderer")
    if self.blurPass then
        context:destroyCustomShaderPass(self.blurPass)
        self.blurPass = nil
    end
    return OF_Result_Success
end

function gaussBlurRender:onApplyParams(context, filter)
    self.blurKernekSize = math.ceil(filter:floatParam("BlurStrength") * 10)
    self.blurIterCount = filter:intParam("BlurStep")

    if filter:enumParam("BlurDirection") == 0 then
        self.blurVertical = true
        self.blurHorizontal = true
    elseif filter:enumParam("BlurDirection") == 1 then
        self.blurVertical = false
        self.blurHorizontal = true
    else
        self.blurVertical = true
        self.blurHorizontal = false
    end

    self.makeGaussKernel(self, 3, self.blurKernekSize)
    return OF_Result_Success
end

function gaussBlurRender:makeGaussKernel(sigma, kernel_size)
    local sqrt_sigma_pi2  = math.sqrt(math.pi * 2.0) * sigma
    local s2 = 2.0 * sigma * sigma
    local sum = 0.0
    for i = 0, kernel_size do
        local value = math.exp(-(i * i) / s2) / sqrt_sigma_pi2
        self.blurKernel:set(i, value)
        if i == 0 then
            sum = sum + value
        else
            sum = sum + value * 2
        end
    end

    -- normalize
    for i = 0, kernel_size do
        self.blurKernel:set(i, self.blurKernel:get(i) / sum)
        --print(i, self.blurKernel:get(i))
    end
    self.blurKernekSize = kernel_size + 1
end

function gaussBlurRender:setGaussIterCount(iterNum)
    self.blurIterCount = iterNum
end

function gaussBlurRender:draw(context, inTex, outTex)
    local width  = outTex.width
    local height = outTex.height
    local cache_tex = context:getTexture(width, height)
    local quad_render = context:sharedQuadRender()

    context:setViewport(0, 0, width, height)
    context:setBlend(false)
    self.blurPass:use()
    self.blurPass:setUniformMatrix4fv("uMVP", 1, 0, Matrix4f.new().x)
    self.blurPass:setUniform1fv("uKernel", self.blurKernel:size(), self.blurKernel)
    self.blurPass:setUniform1i("uKernelSize", self.blurKernekSize)

    local function doBlur(texture)
        context:bindFBO(cache_tex:toOFTexture())
        self.blurPass:setUniform2f("uOnePixel", (self.blurHorizontal and {1.0 / width} or {0.0})[1], 0.0)
        self.blurPass:setUniformTexture("uTexture", 0, texture.textureID, TEXTURE_2D)
    
        quad_render:draw(self.blurPass, false)

        context:bindFBO(outTex)
        self.blurPass:setUniform2f("uOnePixel", 0.0, (self.blurVertical and {1.0 / height} or {0.0})[1])
        self.blurPass:setUniformTexture("uTexture", 0, cache_tex:textureID(), TEXTURE_2D)
    
        quad_render:draw(self.blurPass, false)
    end

    doBlur(inTex)

    for i = 1, self.blurIterCount-1 do
        doBlur(outTex)
    end

    context:releaseTexture(cache_tex)
end


return gaussBlurRender