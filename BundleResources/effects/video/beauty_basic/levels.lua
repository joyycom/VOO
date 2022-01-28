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
TAG = "OrangeFilter-LevelsFilter"
OF_LOGI(TAG, "Call LevelsFilter lua script!")

local vs = [[
precision highp float;
attribute vec4 aPosition;
attribute vec4 aTextureCoord;
varying vec2 vTexCoord;
 
void main()
{
    gl_Position = aPosition;
    vTexCoord = aTextureCoord.xy;
}
]]

local fs = [[
precision mediump float;
varying vec2 vTexCoord;
uniform sampler2D uTexture;

uniform vec3 uLevelMinimum;
uniform vec3 uLevelMiddle;
uniform vec3 uLevelMaximum;
uniform vec3 uMinOutput;
uniform vec3 uMaxOutput;

vec3 GammaCorrection(vec3 color, vec3 gamma)
{
    return pow(color, 1.0 / gamma);
}

vec3 LevelsControlInputRange(vec3 color, vec3 minInput, vec3 maxInput)
{
    return min(max(color - minInput, vec3(0.0)) / (maxInput - minInput), vec3(1.0));
}

vec3 LevelsControlInput(vec3 color, vec3 minInput, vec3 gamma, vec3 maxInput)
{
    return GammaCorrection(LevelsControlInputRange(color, minInput, maxInput), gamma);
}

vec3 LevelsControlOutputRange(vec3 color, vec3 minOutput, vec3 maxOutput)
{
    return mix(minOutput, maxOutput, color);
}

vec3 LevelsControl(vec3 color, vec3 minInput, vec3 gamma, vec3 maxInput, vec3 minOutput, vec3 maxOutput)
{
    return LevelsControlOutputRange(LevelsControlInput(color, minInput, gamma, maxInput), minOutput, maxOutput);
}

void main()
{
    vec4 textureColor = texture2D(uTexture, vTexCoord);
    
    gl_FragColor = vec4(LevelsControl(textureColor.rgb, uLevelMinimum, uLevelMiddle, uLevelMaximum, uMinOutput, uMaxOutput), textureColor.a);
}
]]

local _levelsPass = nil

function initParams(context, filter)
    OF_LOGI(TAG, "call initParams")
    filter:insertFloatParam("Min", 0.0, 1.0, 0.0)
    filter:insertFloatParam("Mid", 0.01, 9.99, 1.0)
    filter:insertFloatParam("Max", 0.0, 1.0, 1.0)
    filter:insertFloatParam("MinOutPut", 0.0, 1.0, 0.0)
    filter:insertFloatParam("MaxOutPut", 0.0, 1.0, 1.0)
    filter:insertIntParam("ChannelMode", 0, 3, 0)

    return OF_Result_Success
end

function initRenderer(context, filter)
    OF_LOGI(TAG, "call initRenderer")
    _levelsPass = context:createCustomShaderPass(vs, fs)
    return OF_Result_Success
end

function teardownRenderer(context, filter)
    OF_LOGI(TAG, "call teardownRenderer")
    context:destroyCustomShaderPass(_levelsPass)
    return OF_Result_Success
end

function applyRGBA(context, filter, frameData, inTex, outTex, debugTex)

    context:setViewport(0, 0, outTex.width, outTex.height)
    context:setBlend(false)
    context:bindFBO(outTex)
    _levelsPass:use()
    _levelsPass:setUniformTexture("uTexture", 0, inTex.textureID, TEXTURE_2D)
    local channelMode = filter:intParam("ChannelMode")
    if 0 == channelMode then
        _levelsPass:setUniform3f("uLevelMinimum", filter:floatParam("Min"), filter:floatParam("Min"), filter:floatParam("Min"))
        _levelsPass:setUniform3f("uLevelMiddle", filter:floatParam("Mid"), filter:floatParam("Mid"), filter:floatParam("Mid"))
        _levelsPass:setUniform3f("uLevelMaximum", filter:floatParam("Max"), filter:floatParam("Max"), filter:floatParam("Max"))
        _levelsPass:setUniform3f("uMinOutput", filter:floatParam("MinOutPut"), filter:floatParam("MinOutPut"), filter:floatParam("MinOutPut"))
        _levelsPass:setUniform3f("uMaxOutput", filter:floatParam("MaxOutPut"), filter:floatParam("MaxOutPut"), filter:floatParam("MaxOutPut"))
    elseif 1 == channelMode then
        _levelsPass:setUniform3f("uLevelMinimum", filter:floatParam("Min"), 0.0, 0.0)
        _levelsPass:setUniform3f("uLevelMiddle", filter:floatParam("Mid"), 1.0, 1.0)
        _levelsPass:setUniform3f("uLevelMaximum", filter:floatParam("Max"), 1.0, 1.0)
        _levelsPass:setUniform3f("uMinOutput", filter:floatParam("MinOutPut"), 0.0, 0.0)
        _levelsPass:setUniform3f("uMaxOutput", filter:floatParam("MaxOutPut"), 1.0, 1.0)
    elseif 2 == channelMode then
        _levelsPass:setUniform3f("uLevelMinimum", 0.0, filter:floatParam("Min"), 0.0)
        _levelsPass:setUniform3f("uLevelMiddle", 1.0, filter:floatParam("Mid"), 1.0)
        _levelsPass:setUniform3f("uLevelMaximum", 1.0, filter:floatParam("Max"), 1.0)
        _levelsPass:setUniform3f("uMinOutput", 0.0, filter:floatParam("MinOutPut"), 0.0)
        _levelsPass:setUniform3f("uMaxOutput", 1.0, filter:floatParam("MaxOutPut"), 1.0)
    else
        _levelsPass:setUniform3f("uLevelMinimum", 0.0, 0.0, filter:floatParam("Min"))
        _levelsPass:setUniform3f("uLevelMiddle", 1.0, 1.0, filter:floatParam("Mid"))
        _levelsPass:setUniform3f("uLevelMaximum", 1.0, 1.0, filter:floatParam("Max"))
        _levelsPass:setUniform3f("uMinOutput", 0.0, 0.0, filter:floatParam("MinOutPut"))
        _levelsPass:setUniform3f("uMaxOutput", 1.0, 1.0, filter:floatParam("MaxOutPut"))
    end
    local quadRender = context:sharedQuadRender()
    quadRender:draw(_levelsPass, false)

    if debugTex then
        context:copyTexture(inTex, debugTex)
    end
    return OF_Result_Success
end

function requiredFrameData(context, game)
    return { OF_RequiredFrameData_None }
end

function onApplyParams(context, filter)
    return OF_Result_Success
end

function readObject(context, filter, archiveIn)
    OF_LOGI(TAG, "call readObject")
    return OF_Result_Success
end

function writeObject(context, filter, archiveOut)
    OF_LOGI(TAG, "call writeObject")
    return OF_Result_Success
end