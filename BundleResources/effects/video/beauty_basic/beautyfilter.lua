--/***************** BEGIN FILE HRADER BLOCK *********************************
--*

--*
--* Copyright (C) 2017-2019 YY.Inc
--* All rights reserved.
--*
--* This library is free software; you can redistribute it and/or modify it
--* under the terms of the GNU Lesser General Public License as published by
--* the Free Software Foundation; either version 3 of the License, or (at
--* your option) any later version. Please review the following information
--* to ensure the GNU Lesser General Public License version 3 requirements
--* will be met: https://www.gnu.org/licenses/lgpl.html.
--*
--* You should have received a copy of the GNU Lesser General Public License
--* along with this library; if not, write to the Free Software Foundation.
--*
--* If you use the source in a your project, please consider send an e-mail
--* to me, and allow me to add your project to my home page.
--*
--***************** END FILE HRADER BLOCK ***********************************/
TAG = "OrangeFilter-BeautyFilter"
OF_LOGI(TAG, "Call BeautyFilter lua script!")

local vs12_15_21_24 = [[
    attribute vec4 aPosition;
    attribute vec4 aTextureCoord;

    uniform highp float texelWidthOffset;
    uniform highp float texelHeightOffset;

    varying vec2 textureCoordinate;
    varying vec4 textureShift_1;
    varying vec4 textureShift_2;
    varying vec4 textureShift_3;
    varying vec4 textureShift_4;

    void main()
    {
        gl_Position = aPosition;

        vec2 singleStepOffset = vec2(texelWidthOffset, texelHeightOffset);
        textureCoordinate = aTextureCoord.xy;
        textureShift_1 = vec4(aTextureCoord.xy - singleStepOffset, aTextureCoord.xy + singleStepOffset);
        textureShift_2 = vec4(aTextureCoord.xy - 2.0 * singleStepOffset, aTextureCoord.xy + 2.0 * singleStepOffset);
        textureShift_3 = vec4(aTextureCoord.xy - 3.0 * singleStepOffset, aTextureCoord.xy + 3.0 * singleStepOffset);
        textureShift_4 = vec4(aTextureCoord.xy - 4.0 * singleStepOffset, aTextureCoord.xy + 4.0 * singleStepOffset);
    }
]]

local fs12_15_21_24 = [[
    precision mediump float;
    uniform sampler2D inputImageTexture;

    varying highp vec2 textureCoordinate;
    varying highp vec4 textureShift_1;
    varying highp vec4 textureShift_2;
    varying highp vec4 textureShift_3;
    varying highp vec4 textureShift_4;

    void main()
    {
        mediump vec3 sum = texture2D(inputImageTexture, textureCoordinate).rgb;
        sum += texture2D(inputImageTexture, textureShift_1.xy).rgb;
        sum += texture2D(inputImageTexture, textureShift_1.zw).rgb;
        sum += texture2D(inputImageTexture, textureShift_2.xy).rgb;
        sum += texture2D(inputImageTexture, textureShift_2.zw).rgb;
        sum += texture2D(inputImageTexture, textureShift_3.xy).rgb;
        sum += texture2D(inputImageTexture, textureShift_3.zw).rgb;
        sum += texture2D(inputImageTexture, textureShift_4.xy).rgb;
        sum += texture2D(inputImageTexture, textureShift_4.zw).rgb;

        gl_FragColor = vec4(sum * 0.1111, 1.0);
    }
]]

local vs18 = [[
    attribute vec4 aPosition;
    attribute vec2 aTextureCoord;
    varying vec2 textureCoordinate;
    varying vec2 textureCoordinate2;
    void main() {
        gl_Position = aPosition;
        textureCoordinate = aTextureCoord;
        textureCoordinate2 = aTextureCoord;
    }
]]

local fs18 = [[
    precision mediump float;
    varying vec2 textureCoordinate;
    varying vec2 textureCoordinate2;
    uniform sampler2D inputImageTexture;
    uniform sampler2D inputImageTexture2;
    uniform float uVarFacrtor;
    uniform float uRoi;
    void main()
    {
        lowp vec3 iColor = texture2D(inputImageTexture, textureCoordinate).rgb;
        lowp vec3 meanColor = texture2D(inputImageTexture2, textureCoordinate2).rgb;
        mediump vec3 diffColor = iColor - meanColor;
        mediump float var = dot(diffColor, diffColor) * uVarFacrtor;//16.7;
        var = min(var, 1.0);
    
        lowp float roi = uRoi;//0.8;
    
        gl_FragColor = vec4(var, roi, 0.0, 1.0);
    }
]]

local vs27 = [[
    precision highp float;
    attribute vec4 aPosition;
    attribute vec2 aTextureCoord;
    varying highp vec2 textureCoordinate;
    varying highp vec2 textureCoordinate2;
    varying highp vec2 textureCoordinate3;
    void main() {
        gl_Position = aPosition;
        textureCoordinate = aTextureCoord;
        textureCoordinate2 = aTextureCoord;
        textureCoordinate3 = aTextureCoord;
    }
]]

local fs27 = [[
    precision mediump float;
    varying highp vec2 textureCoordinate;
    varying highp vec2 textureCoordinate2;
    varying highp vec2 textureCoordinate3;

    uniform sampler2D inputImageTexture;
    uniform sampler2D inputImageTexture2;
    uniform sampler2D inputImageTexture3;

    uniform lowp float blurAlpha;

    void main()
    {
        lowp vec4 iColor = texture2D(inputImageTexture, textureCoordinate);
        lowp vec3 meanColor = texture2D(inputImageTexture2, textureCoordinate2).rgb;
        lowp vec3 varColor = texture2D(inputImageTexture3, textureCoordinate3).rgb;

        mediump float theta = 0.1 * blurAlpha + 0.001;
        mediump float p = clamp((min(iColor.r, meanColor.r - 0.1) - 0.2) * 4.0, 0.0, 1.0);
        mediump float meanVar = varColor.r;
        mediump float kMin;

        lowp vec3 resultColor;
        kMin = (theta / (meanVar + theta)) * p;
        kMin = kMin * (0.5 + 0.5 * blurAlpha);
        kMin = min(kMin, varColor.g);

        resultColor = mix(iColor.rgb, meanColor, kMin);
        gl_FragColor = vec4(resultColor, iColor.a);
    }
]]


local _pass12_15_21_24 = nil
local _pass18 = nil
local _pass27 = nil
local _opacity = 0
local _setByMessage = false

function initParams(context, filter)
    OF_LOGI(TAG, "call initParams")
    filter:insertFloatParam("Opacity", 0.0, 1.0, 0.99)
    filter:insertFloatParam("TexelWidthOffset", 0.0, 10.0, 1.5)
    filter:insertFloatParam("TexelHeightOffset", 0.0, 10.0, 2.0)
    filter:insertFloatParam("VarFacrtor", 0.0, 50.0, 16.7)
    filter:insertFloatParam("Roi", 0.0, 1.0, 0.8)
    return OF_Result_Success
end

function onApplyParams(context, filter)
    if not _setByMessage then
        _opacity = filter:floatParam("Opacity")
    end
    return OF_Result_Success
end

function initRenderer(context, filter)
    OF_LOGI(TAG, "call initRenderer")

    _pass12_15_21_24 = context:createCustomShaderPass(vs12_15_21_24, fs12_15_21_24)
    _pass18 = context:createCustomShaderPass(vs18, fs18)
    _pass27 = context:createCustomShaderPass(vs27, fs27)
    return OF_Result_Success
end

function teardownRenderer(context, filter)
    OF_LOGI(TAG, "call teardownRenderer")
    context:destroyCustomShaderPass(_pass12_15_21_24)
    context:destroyCustomShaderPass(_pass18)
    context:destroyCustomShaderPass(_pass27)

    return OF_Result_Success
end

function applyRGBA(context, filter, frameData, inTex, outTex, debugTex)

    local width = outTex.width
    local height = outTex.height
    local halfWidth = width / 2
    local halfHeight = height / 2

    local _tempTex0 = context:getTexture(halfWidth, halfHeight)
    local _tempTex1 = context:getTexture(halfWidth, halfHeight)
    local _tempTex2 =  context:getTexture(halfWidth, halfHeight)
    
    local tempTex0 = _tempTex0:toOFTexture()
    local tempTex1 = _tempTex1:toOFTexture()
    local tempTex2 = _tempTex2:toOFTexture()

    local quadRender = context:sharedQuadRender()
    context:setBlend(false)

    -- Pass 0. 12
    context:setViewport(0, 0, halfWidth, halfHeight)
    context:bindFBO(tempTex0)
    _pass12_15_21_24:use()
    _pass12_15_21_24:setUniformTexture("inputImageTexture", 0, inTex.textureID, TEXTURE_2D)
    _pass12_15_21_24:setUniform1f("texelWidthOffset", 0.0)
    _pass12_15_21_24:setUniform1f("texelHeightOffset", filter:floatParam("TexelHeightOffset")/halfHeight)
    -- _pass12_15_21_24:setUniform1f("texelHeightOffset", 0.003125)
    quadRender:draw(_pass12_15_21_24, false)

    -- Pass 1. 15
    context:bindFBO(tempTex2)
    _pass12_15_21_24:use()
    _pass12_15_21_24:setUniformTexture("inputImageTexture", 0, _tempTex0:textureID(), TEXTURE_2D)
    _pass12_15_21_24:setUniform1f("texelWidthOffset", filter:floatParam("TexelWidthOffset")/halfWidth)
    -- _pass12_15_21_24:setUniform1f("texelWidthOffset", 0.004167)
    _pass12_15_21_24:setUniform1f("texelHeightOffset", 0.0)
    quadRender:draw(_pass12_15_21_24, false)

    -- Pass 2. 18
    context:bindFBO(tempTex0)
    _pass18:use()
    _pass18:setUniformTexture("inputImageTexture", 0, inTex.textureID, TEXTURE_2D)
    _pass18:setUniformTexture("inputImageTexture2", 1, _tempTex2:textureID(), TEXTURE_2D)
    _pass18:setUniform1f("uVarFacrtor", filter:floatParam("VarFacrtor"))
    _pass18:setUniform1f("uRoi", filter:floatParam("Roi"))
    quadRender:draw(_pass18, false)

    -- Pass 3. 21
    context:bindFBO(tempTex1)
    _pass12_15_21_24:use()
    _pass12_15_21_24:setUniformTexture("inputImageTexture", 0, _tempTex0:textureID(), TEXTURE_2D)
    _pass12_15_21_24:setUniform1f("texelWidthOffset", 0.000000)
    _pass12_15_21_24:setUniform1f("texelHeightOffset", filter:floatParam("TexelHeightOffset")/halfHeight)
    -- _pass12_15_21_24:setUniform1f("texelHeightOffset", 0.003125)
    quadRender:draw(_pass12_15_21_24, false)

    -- Pass 4. 24
    context:bindFBO(tempTex0)
    _pass12_15_21_24:use()
    _pass12_15_21_24:setUniformTexture("inputImageTexture", 0, _tempTex1:textureID(), TEXTURE_2D)
    _pass12_15_21_24:setUniform1f("texelWidthOffset", filter:floatParam("TexelWidthOffset")/halfWidth)
    -- _pass12_15_21_24:setUniform1f("texelWidthOffset", 0.004167)
    _pass12_15_21_24:setUniform1f("texelHeightOffset", 0.0)
    quadRender:draw(_pass12_15_21_24, false)

    -- Pass 5. 27
    context:setViewport(0, 0, width, height)
    context:bindFBO(outTex)
    _pass27:use()
    _pass27:setUniformTexture("inputImageTexture", 0, inTex.textureID, TEXTURE_2D)
    _pass27:setUniformTexture("inputImageTexture2", 1, _tempTex2:textureID(), TEXTURE_2D)
    _pass27:setUniformTexture("inputImageTexture3", 2, _tempTex0:textureID(), TEXTURE_2D)
    _pass27:setUniform1f("blurAlpha", _opacity)
    quadRender:draw(_pass27, false)
    
    if debugTex ~= nil then
        context:copyTexture(tempTex2, debugTex)
    end
    
    context:releaseTexture(_tempTex0)
    context:releaseTexture(_tempTex1)
    context:releaseTexture(_tempTex2)
    
    return OF_Result_Success
end

function requiredFrameData(context, game)
    return { OF_RequiredFrameData_None }
end

function readObject(context, filter, archiveIn)
    OF_LOGI(TAG, "call readObject")
    return OF_Result_Success
end

function writeObject(context, filter, archiveOut)
    OF_LOGI(TAG, "call writeObject")
    return OF_Result_Success
end

function onReceiveMessage(context, filter, msg)
	OF_LOGI("beautyfilter", string.format("call onReceiveMessage %s", msg))
    local tab = Json.JsonToTable(msg)
	if tab.beauty then
		_opacity = tab.beauty / 100
        _setByMessage = true
	end
	return ""
end
