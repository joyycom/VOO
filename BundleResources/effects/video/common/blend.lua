
local TAG = "BlendRender"

local BlendIndextTable =
{
    1, 8, 2, 7, 12, 14, 13, 5, 4, 9, 3, 21, 23, 16, 10, 19, 11, 6, 15, 17, 18, 20, 22 
}

local BlendRender = {
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
    fs_common1 = [[
	precision mediump float;
	uniform sampler2D uTexture0;
	uniform sampler2D uTexture1;
	uniform float uOpacity;
	varying vec2 vTexCoord;

	void main()
	{
		vec4 base = texture2D(uTexture0, vTexCoord);
		vec4 blend = texture2D(uTexture1, vTexCoord);
        //blend.a = blend.a * uOpacity;
        vec4 res = base;
    ]],
    fs_common2 = [[
        res.rgb = min(vec3(1.0), max(vec3(0.0), res.rgb));
        res.a = blend.a;
        gl_FragColor = mix(base, res, blend.a * uOpacity); 
    }   
    ]],

    blend_fs = {
    [[
        res = blend;
    ]],--normal
    [[
        res.rgb = min(base.rgb, blend.rgb) + blend.rgb * (1.0 - base.a) + base.rgb * (1.0 - blend.a);
        res.a = base.a + blend.a - base.a * blend.a;
    ]],--darken 
    [[
        res = base * blend;
    ]], --multiply
    [[
        res.r = ((blend.r == 0.0) ? blend.r : max((1.0 - ((1.0 - base.r) / blend.r)), 0.0));
        res.g = ((blend.g == 0.0) ? blend.g : max((1.0 - ((1.0 - base.g) / blend.g)), 0.0));
        res.b = ((blend.b == 0.0) ? blend.b : max((1.0 - ((1.0 - base.b) / blend.b)), 0.0));
        res.a = ((blend.a == 0.0) ? blend.a : max((1.0 - ((1.0 - base.a) / blend.a)), 0.0));
    ]], --colorburn
    [[
        res = max(base + blend - vec4(1.0), vec4(0.0));
    ]],--linearburn
    [[
        res = ((base.r + base.g + base.b) > (blend.r + blend.g + blend.b)) ? blend : base;
    ]],--darkencolor
    [[
        res.rgb = max(base.rgb, blend.rgb) + blend.rgb * (1.0 - base.a) + base.rgb * (1.0 - blend.a);
        res.a = base.a + blend.a - base.a * blend.a;
    ]],--lighten
    [[
        res = vec4(1.0) - ((vec4(1.0) - base) * (vec4(1.0)- blend));
    ]],--screen    
    [[
        res.r = (blend.r == 1.0) ? 1.0 : base.r/(1.0 - blend.r);
        res.g = (blend.g == 1.0) ? 1.0 : base.g/(1.0 - blend.g);
        res.b = (blend.b == 1.0) ? 1.0 : base.b/(1.0 - blend.b);
        res.a = (blend.a == 1.0) ? 1.0 : base.a/(1.0 - blend.a);
    ]], --colordodge
    [[
        res = base + blend;
    ]],--lineardodge
    [[
        res = ((base.r + base.g + base.b) > (blend.r + blend.g + blend.b)) ? base : blend;
    ]],--lightencolor
    [[
        res.r = (base.r < 0.5 ? (2.0 * base.r * blend.r) : (1.0 - 2.0 * (1.0 - base.r) * (1.0 - blend.r)));
        res.g = (base.g < 0.5 ? (2.0 * base.g * blend.g) : (1.0 - 2.0 * (1.0 - base.g) * (1.0 - blend.g)));
        res.b = (base.b < 0.5 ? (2.0 * base.b * blend.b) : (1.0 - 2.0 * (1.0 - base.b) * (1.0 - blend.b)));
        res.a = base.a + blend.a - base.a * blend.a;
    ]],--overlay
    [[
        res.r = (blend.r < 0.5)
            ? (2.0 * base.r * blend.r + base.r * base.r * (1.0 - 2.0 * blend.r))
            : (sqrt(base.r) * (2.0 * blend.r - 1.0) + 2.0 * base.r * (1.0 - blend.r));
        res.g = (blend.g < 0.5)
            ? (2.0 * base.g * blend.g + base.g * base.g * (1.0 - 2.0 * blend.g))
            : (sqrt(base.g) * (2.0 * blend.g - 1.0) + 2.0 * base.g * (1.0 - blend.g));
        res.b = (blend.b < 0.5)
            ? (2.0 * base.b * blend.b + base.b * base.b * (1.0 - 2.0 * blend.b))
            : (sqrt(base.b) * (2.0 * blend.b - 1.0) + 2.0 * base.b * (1.0 - blend.b));
        res.a = (blend.a < 0.5)
            ? (2.0 * base.a * blend.a + base.a * base.a * (1.0 - 2.0 * blend.a))
            : (sqrt(base.a) * (2.0 * blend.a - 1.0) + 2.0 * base.a * (1.0 - blend.a));
    ]],--softlight
    [[
        res.r = (blend.r < 0.5 ? (2.0 * blend.r * base.r) : (1.0 - 2.0 * (1.0 - blend.r) * (1.0 - base.r)));
        res.g = (blend.g < 0.5 ? (2.0 * blend.g * base.g) : (1.0 - 2.0 * (1.0 - blend.g) * (1.0 - base.g)));
        res.b = (blend.b < 0.5 ? (2.0 * blend.b * base.b) : (1.0 - 2.0 * (1.0 - blend.b) * (1.0 - base.b)));
        res.a = (blend.a < 0.5 ? (2.0 * blend.a * base.a) : (1.0 - 2.0 * (1.0 - blend.a) * (1.0 - base.a)));
    ]],--hardlight
    [[
        res.r = (blend.r < 0.5)
            ? (1.0 - (1.0 - base.r) / (2.0 * blend.r))
            : (base.r / (1.0 - 2.0 * (blend.r - 0.5)));
        res.g = (blend.g < 0.5)
            ? (1.0 - (1.0 - base.g) / (2.0 * blend.g))
            : (base.g / (1.0 - 2.0 * (blend.g - 0.5)));
        res.b = (blend.b < 0.5)
            ? (1.0 - (1.0 - base.b) / (2.0 * blend.b))
            : (base.b / (1.0 - 2.0 * (blend.b - 0.5)));
        res.a = (blend.a < 0.5)
            ? (1.0 - (1.0 - base.a) / (2.0 * blend.a))
            : (base.a / (1.0 - 2.0 * (blend.a - 0.5)));
    ]],--vivid light
    [[
        res = base + vec4(2.0) * blend - vec4(1.0);
    ]],--linearlight
    [[
        res.r = (blend.r < 0.5) ? min(base.r, 2.0 * blend.r) : max(base.r, 2.0 * (blend.r - 0.5));
        res.g = (blend.g < 0.5) ? min(base.g, 2.0 * blend.g) : max(base.g, 2.0 * (blend.g - 0.5));
        res.b = (blend.b < 0.5) ? min(base.b, 2.0 * blend.b) : max(base.b, 2.0 * (blend.b - 0.5));
        res.a = (blend.a < 0.5) ? min(base.a, 2.0 * blend.a) : max(base.a, 2.0 * (blend.a - 0.5));
    ]],--pin light
    [[
        res = base + blend;
        res.r = (res.r >= 1.0) ? 1.0 : 0.0;
        res.g = (res.g >= 1.0) ? 1.0 : 0.0;
        res.b = (res.b >= 1.0) ? 1.0 : 0.0;
        res.a = (res.a >= 1.0) ? 1.0 : 0.0;
    ]],--Hard Mix
    [[
        res = abs(blend - base);
    ]],--difference
    [[
        res = 0.5 - 2.0 * (base - vec4(0.5)) * (blend - vec4(0.5));
    ]],--Exclusion
    [[
        res = base - blend;
    ]], --substract.
    [[
        res = base / blend;
    ]], --divide
    [[
        res = base + blend;
    ]], --add
    },

    blendPass = {},
    blendMode = 0,  --default blend normal
    opacity = 100.0,
}

function BlendRender:initParams(context, filter)
    filter:insertEnumParam("BlendMode", 0, { "None", "Screen", "Darken", "Lighten", "Overlay",
    "HardLight", "SoftLight", "LinearBurn", "ColorBurn", "ColorDodge", "Multiply", "Subtract", 
    "Add", "LinearLight", "LinearDodge", "Difference", "LighterColor", "DarkerColor", "VividLight", "PinLight", "HardMix", "Exclusion", "Divide"})
    
    -- params for of blend choose better
    filter:insertBoolParam("BlendEnableSorted", false);
    filter:insertEnumParam("SortedBlendMode", 0, { "Normal", "Darken", "Multiply", "CorlorBurn", "LinearBurn",
    "DarkerColor", "Lighten", "Screen", "ColorDodge", "LinearDodge", "LighterColor", "Overlay", 
    "Softlight", "Hardlight", "Vividlight", "Linearlight", "Pinlight", "HardMix", "Difference", "Exclusion", "Substract", "Divide", "Add" })
    
    filter:insertIntParam("BlendOpacity", 0, 100, 100)

    for i = 1, #self.blend_fs do
        self.blendPass[i] = nil
    end
    return OF_Result_Success
end

function BlendRender:initRenderer(context, filter)
    OF_LOGI(TAG, "call initRenderer")
    return OF_Result_Success
end

function BlendRender:onApplyParams(context, filter)
    if filter:boolParam("BlendEnableSorted") then
        self.blendMode = filter:enumParam("SortedBlendMode") + 1 
    else    
        self.blendMode = BlendIndextTable[filter:enumParam("BlendMode")+1]
    end
    self.opacity = filter:intParam("BlendOpacity")
    return OF_Result_Success
end

function BlendRender:teardown(context)
    OF_LOGI(TAG, "call BlendRender teardownRenderer")
    for i = 1, #self.blend_fs do
        if self.blendPass[i] ~= nil then
            context:destroyCustomShaderPass(self.blendPass[i])
            self.blendPass[i] = nil
        end
    end
    return OF_Result_Success
end

function BlendRender:setOpacity(opacity)
    self.opacity = opacity
end

function BlendRender:setBlendMode(mode)
    self.blendMode = mode
end

function BlendRender:draw(context, baseTex, blendTex, outTex, blendMat)
    context:bindFBO(outTex)
    context:setViewport(0, 0, outTex.width, outTex.height)
    context:setBlend(false)
    
    if self.blendPass[self.blendMode] == nil then
        -- OF_LOGI(TAG, string.format("createCustomShaderPass mode = %d",self.blendMode))
        self.blendPass[self.blendMode] = context:createCustomShaderPass(self.vs, self.fs_common1..self.blend_fs[self.blendMode]..self.fs_common2)
    end
    
    local blendPass = self.blendPass[self.blendMode]
    blendPass:use()
	blendPass:setUniformMatrix4fv("uMVP", 1, 0, blendMat.x)
	blendPass:setUniformTexture("uTexture0", 0, baseTex.textureID, TEXTURE_2D)
	blendPass:setUniformTexture("uTexture1", 1, blendTex.textureID, TEXTURE_2D)
	blendPass:setUniform1f("uOpacity", self.opacity / 100)

    local quadRender = context:sharedQuadRender()
    quadRender:draw(blendPass, false)

    return OF_Result_Success
end

return BlendRender