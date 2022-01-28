local TAG = "TextureRender"
local TextureRender = {
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
    fs_animate = [[
        precision highp float;
        varying vec2 vTexCoord;

        uniform float uTileX;
        uniform float uTileY;
        uniform float uAnimFPS;
        uniform float uTimestamp;
        uniform sampler2D uTexture0;
        uniform vec4 uColor;
        
        void main()
        {
            vec2 uv = vTexCoord;
            int idx = int(mod(uTimestamp * uAnimFPS / 1000.0, uTileX * uTileY));
            int rowIdx = int(mod(float(idx) / uTileX, uTileY));
            int colIdx = int(mod(float(idx), uTileX));

            uv.x = uv.x / uTileX + float(colIdx) / uTileX;
            uv.y = uv.y / uTileY + float(rowIdx) / uTileY;

            vec4 color = texture2D(uTexture0, uv);
            gl_FragColor = color * uColor;
        }
        ]],

   fs_simple = [[
        precision highp float;
        varying vec2 vTexCoord;

        uniform sampler2D uTexture0;
        uniform vec4 uColor;
        
        void main()
        {
            vec4 color = texture2D(uTexture0, vTexCoord);
            gl_FragColor = color * uColor;
        }
        ]],
    
    animatePass = nil,
    simplePass = nil,
    color = Vec4f.new(1.0, 1.0, 1.0, 1.0),
    tileX = 1,
    tileY = 1,
    animFPS = 1,
    timestamp = 0
}

function TextureRender:initParams(context, filter)
    filter:insertColorParam("Color", self.color)
    filter:insertIntParam("TileX", 1, 20, self.tileX)
	filter:insertIntParam("TileY", 1, 20, self.tileY)
	filter:insertIntParam("FPS", 1, 60, self.animFPS)
end

function TextureRender:initRenderer(context, filter)
    OF_LOGI(TAG, "call initParams")
    self.animatePass = context:createCustomShaderPass(self.vs, self.fs_animate)
    self.simplePass = context:createCustomShaderPass(self.vs, self.fs_simple)
    
    return OF_Result_Success
end

function TextureRender:onApplyParams(context, filter)
    self.color = filter:colorParam("Color")
    self.tileX = filter:intParam("TileX")
    self.tileY = filter:intParam("TileY")
    self.animFPS = filter:intParam("FPS")
    return OF_Result_Success
end

function TextureRender:teardown(context, filter)
    OF_LOGI(TAG, "call teardownRenderer")
    if self.animatePass then
        context:destroyCustomShaderPass(self.animatePass)
        self.animatePass = nil
    end
    if self.simplePass then
        context:destroyCustomShaderPass(self.simplePass)
        self.simplePass = nil
    end
    return OF_Result_Success
end

function TextureRender:setColor(color)
    self.color = color
end

function TextureRender:setOpacity(opacity)
    self.color.w = opacity
end

function TextureRender:setAnimationData(tileX, tileY, animFPS, timestamp)
    self.tileX = tileX
    self.tileY = tileY
    self.animFPS = animFPS
    self.timestamp = timestamp
end

function TextureRender:draw(context, inTex, outTex, texMat)
    context:bindFBO(outTex)
    context:setViewport(0, 0, outTex.width, outTex.height)

    self.simplePass:use()
    self.simplePass:setUniformTexture("uTexture0", 0, inTex.textureID, TEXTURE_2D)
    self.simplePass:setUniform4f("uColor", self.color.x, self.color.y, self.color.z, self.color.w)
    self.simplePass:setUniformMatrix4fv("uMVP", 1, 0, texMat.x)

    local quadRender = context:sharedQuadRender()
    quadRender:draw(self.simplePass, false)
end

function TextureRender:drawWithAnimation(context, inTex, outTex, texMat)
    context:bindFBO(outTex)
    context:setViewport(0, 0, outTex.width, outTex.height)

    self.animatePass:use()
    self.animatePass:setUniformTexture("uTexture0", 0, inTex.textureID, TEXTURE_2D)
    self.animatePass:setUniform4f("uColor", self.color.x, self.color.y, self.color.z, self.color.w)
    self.animatePass:setUniformMatrix4fv("uMVP", 1, 0, texMat.x)
    self.animatePass:setUniform1f("uTileX", self.tileX)
    self.animatePass:setUniform1f("uTileY", self.tileY)
    self.animatePass:setUniform1f("uAnimFPS", self.animFPS)
    self.animatePass:setUniform1f("uTimestamp", self.timestamp)

    local quadRender = context:sharedQuadRender()
    quadRender:draw(self.animatePass, false)
end

return TextureRender