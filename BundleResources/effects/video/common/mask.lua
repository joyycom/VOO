local TAG = "MaskRender"
local MaskRender = {
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
        precision mediump float;
        uniform sampler2D uBaseTex;
        uniform sampler2D uMaskTex;
        uniform float uTileX;
        uniform float uTileY;
        uniform float uAnimFPS;
        uniform float uTimestamp;
        uniform float uOpacity;
        uniform int uInverse;

        varying vec2 vTexCoord;

        void main()
        {
            vec2 uv = vTexCoord;
            int idx = int(mod(uTimestamp * uAnimFPS / 1000.0, uTileX * uTileY));
            int rowIdx = int(mod(float(idx) / uTileX, uTileY));
            int colIdx = int(mod(float(idx), uTileX));

            uv.x = uv.x / uTileX + float(colIdx) / uTileX;
            uv.y = uv.y / uTileY + float(rowIdx) / uTileY;

            vec4 color = texture2D(uBaseTex, vTexCoord) * uOpacity;
            float mask = texture2D(uMaskTex, uv).r;
            
            float inverse = float(uInverse);
            mask = mask * step(0.5, inverse) + (1.0 - mask) * step(inverse, 0.5);
            //vec3 finalColor = mix(vec3(1.0), color.rgb, mask);

            gl_FragColor = vec4(color.rgb, mask * color.a);
        }
        ]],
    fs_simple = [[
        precision mediump float;
        uniform sampler2D uBaseTex;
        uniform sampler2D uMaskTex;
        uniform int uInverse;
        uniform float uOpacity;
        varying vec2 vTexCoord;

        void main()
        {
            vec4 color = texture2D(uBaseTex, vTexCoord) * uOpacity;
            float mask = texture2D(uMaskTex, vTexCoord).r;
            
            float inverse = float(uInverse);
            mask = mask * step(0.5, inverse) + (1.0 - mask) * step(inverse, 0.5);
            //vec3 finalColor = mix(vec3(1.0), color.rgb, mask);

            gl_FragColor = vec4(color.rgb, mask * color.a);
        }
        ]],
    maskAnimatePass = nil,
    maskPass = nil,
    maskEnable = false,
    inverseEnable = false,
    tileX = 1,
    tileY = 1,
    animFPS = 1,
    timestamp = 0,
    opacity = 1.0
}

function MaskRender:initParams(context, filter)
    filter:insertBoolParam("Mask", self.maskEnable)
    filter:insertBoolParam("MaskInverse", self.inverseEnable)
end

function MaskRender:onApplyParams(context, filter)
    self.maskEnable = filter:boolParam("Mask")
    self.inverseEnable = filter:boolParam("MaskInverse")
end

function MaskRender:initRenderer(context, filter)
    OF_LOGI(TAG, "call MaskRender initRenderer")
    self.maskPass = context:createCustomShaderPass(self.vs, self.fs_simple)
    self.maskAnimatePass = context:createCustomShaderPass(self.vs, self.fs_animate)
end

function MaskRender:teardown(context, filter)
    OF_LOGI(TAG, "call MaskRender teardownRenderer")
    if self.maskPass then
        context:destroyCustomShaderPass(self.maskPass)
        self.maskPass = nil
    end
    if self.maskAnimatePass then
        context:destroyCustomShaderPass(self.maskAnimatePass)
        self.maskAnimatePass = nil
    end
end

function MaskRender:setInverseEnabled(enable)
    self.inverseEnable = enable
end

function MaskRender:setAnimationData(tileX, tileY, animFPS, timestamp)
    self.tileX = tileX
    self.tileY = tileY
    self.animFPS = animFPS
    self.timestamp = timestamp
end

function MaskRender:setOpacity(opacity)
    self.opacity = opacity
end

-- if inTex == nil , then maskTex render same size to outTex, maskMat should be Matrix4f:ScaleMat(1.0, 1.0, 1.0)
-- else maskTex render same size with inTex, according to customize maskMat
function MaskRender:draw(context, inTex, maskTex, outTex, maskMat)
    -- if outside caller has not copied inTex to outTex then
    if not self.maskEnable  or  maskTex == nil then
        return
    end

    local temp = nil
    if inTex == nil then
        temp = context:getTexture(outTex.width, outTex.height)
        inTex = temp:toOFTexture()
        context:copyTexture(outTex, inTex)
    end
    
    context:bindFBO(outTex)
    context:setViewport(0, 0, outTex.width, outTex.height)
    context:setBlend(false)
    
    self.maskPass:use()
    self.maskPass:setUniformTexture("uBaseTex", 0, inTex.textureID, TEXTURE_2D)
    self.maskPass:setUniformTexture("uMaskTex", 1, maskTex.textureID, TEXTURE_2D)
    self.maskPass:setUniform1i("uInverse", (self.inverseEnable and 1) or 0)
    self.maskPass:setUniform1f("uOpacity", self.opacity)
    self.maskPass:setUniformMatrix4fv("uMVP", 1, 0, maskMat.x)

    local quadRender = context:sharedQuadRender()
    quadRender:draw(self.maskPass, false)

    context:setBlend(false)

    if temp ~= nil then context:releaseTexture(temp)  end
end

function MaskRender:drawWithAnimation(context, inTex, maskTex, outTex, maskMat)
    -- if outside caller has not copied inTex to outTex then
    if not self.maskEnable  or  maskTex == nil then
        return
    end

    local temp = nil
    if inTex == nil then
        temp = context:getTexture(outTex.width, outTex.height)
        inTex = temp:toOFTexture()
        context:copyTexture(outTex, inTex)
    end

    context:bindFBO(outTex)
    context:setViewport(0, 0, outTex.width, outTex.height)
    context:setBlend(false)
    
    self.maskAnimatePass:use()
    self.maskPass:setUniformTexture("uBaseTex", 0, inTex.textureID, TEXTURE_2D)
    self.maskAnimatePass:setUniformTexture("uMaskTex", 1, maskTex.textureID, TEXTURE_2D)
    self.maskAnimatePass:setUniform1i("uInverse", (self.inverseEnable and 1) or 0)
    self.maskAnimatePass:setUniform1f("uTileX", self.tileX)
    self.maskAnimatePass:setUniform1f("uTileY", self.tileY)
    self.maskAnimatePass:setUniform1f("uAnimFPS", self.animFPS)
    self.maskAnimatePass:setUniform1f("uOpacity", self.opacity)
    self.maskAnimatePass:setUniform1f("uTimestamp", self.timestamp)
    self.maskAnimatePass:setUniformMatrix4fv("uMVP", 1, 0, maskMat.x)

    local quadRender = context:sharedQuadRender()
    quadRender:draw(self.maskAnimatePass, false)

    if temp ~= nil then context:releaseTexture(temp)  end
end

return MaskRender