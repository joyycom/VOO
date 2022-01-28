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
TAG = "OrangeFilter-LookUpTableFilter"
OF_LOGI(TAG, "Call LookUpTableFilter lua script!")

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
precision highp float;
varying highp vec2 vTexCoord;
 
uniform sampler2D uTextureIn;
uniform sampler2D uTextureLookupTable;

uniform highp float uIntensity;
uniform bool uUseCustomLinearFilter;

lowp vec3 texel;
lowp vec3 color;
highp float blueColor;
highp vec2 quad1;
highp vec2 quad2;
highp vec2 texPos1;
highp vec2 texPos2;
lowp vec4 newColor1;
lowp vec4 newColor2;

const mediump float c_textureSize = 64.0;
const mediump float c_texelSize = 1.0 / c_textureSize;

// https://www.codeproject.com/Articles/236394/Bi-Cubic-and-Bi-Linear-Interpolation-with-GLSL
// Function to get interpolated texel data from a texture with GL_NEAREST property.
// Bi-Linear interpolation is implemented in this function with the 
// help of nearest four data.
vec4 tex2DBiLinear( sampler2D textureSampler_i, vec2 texCoord_i )
{
	texCoord_i -= c_texelSize*0.5; // must shitf origin texture coord to ensure it's in center.

    vec4 p0q0 = texture2D(textureSampler_i, texCoord_i);
    vec4 p1q0 = texture2D(textureSampler_i, texCoord_i + vec2(c_texelSize, 0));

    vec4 p0q1 = texture2D(textureSampler_i, texCoord_i + vec2(0, c_texelSize));
    vec4 p1q1 = texture2D(textureSampler_i, texCoord_i + vec2(c_texelSize, c_texelSize));

    float a = fract( texCoord_i.x * c_textureSize ); // Get Interpolation factor for X direction. Fraction near to valid data.

    vec4 pInterp_q0 = mix( p0q0, p1q0, a ); // Interpolates top row in X direction.
    vec4 pInterp_q1 = mix( p0q1, p1q1, a ); // Interpolates bottom row in X direction.

    float b = fract( texCoord_i.y * c_textureSize );// Get Interpolation factor for Y direction.
    return mix( pInterp_q0, pInterp_q1, b ); // Interpolate in Y direction.
}

void main()
{
    texel=texture2D(uTextureIn, vTexCoord).rgb;
	blueColor = texel.b * 15.0;
    quad1.y = floor(floor(blueColor) / 4.0);
    quad1.x = floor(blueColor) - (quad1.y * 4.0);
    quad2.y = floor(ceil(blueColor) / 4.0);
    quad2.x = ceil(blueColor) - (quad2.y * 4.0);
    texPos1.x = (quad1.x * 0.25) + 0.5/c_textureSize + ((0.25 - 1.0/c_textureSize) * texel.r);
    texPos1.y = (quad1.y * 0.25) + 0.5/c_textureSize + ((0.25 - 1.0/c_textureSize) * texel.g);
    texPos2.x = (quad2.x * 0.25) + 0.5/c_textureSize + ((0.25 - 1.0/c_textureSize) * texel.r);
    texPos2.y = (quad2.y * 0.25) + 0.5/c_textureSize + ((0.25 - 1.0/c_textureSize) * texel.g);

	if(uUseCustomLinearFilter)
	{
		newColor1 = tex2DBiLinear(uTextureLookupTable, texPos1);
		newColor2 = tex2DBiLinear(uTextureLookupTable, texPos2);
	}
    else
	{
		newColor1 = texture2D(uTextureLookupTable, texPos1);
		newColor2 = texture2D(uTextureLookupTable, texPos2);
	}
	
    color = mix(newColor1.rgb, newColor2.rgb, fract(blueColor));

	texel = mix(texel, color, uIntensity);
    gl_FragColor = vec4(texel.rgb, 1.0);
}
]]

local fs_512 =[[
 precision mediump float;
 varying vec2 vTexCoord;
 uniform sampler2D uTextureIn;
 uniform sampler2D uTextureLookupTable;
 
 uniform lowp float uIntensity;

 void main()
 {
     highp vec4 textureColor = texture2D(uTextureIn, vTexCoord);
     highp float blueColor = textureColor.b * 63.0;
     
     highp vec2 quad1;
     quad1.y = floor(floor(blueColor) / 8.0);
     quad1.x = floor(blueColor) - (quad1.y * 8.0);
     
     highp vec2 quad2;
     quad2.y = floor(ceil(blueColor) / 8.0);
     quad2.x = ceil(blueColor) - (quad2.y * 8.0);
     
     highp vec2 texPos1;
     texPos1.x = (quad1.x * 0.125) + 0.5/512.0 + ((0.125 - 1.0/512.0) * textureColor.r);
     texPos1.y = (quad1.y * 0.125) + 0.5/512.0 + ((0.125 - 1.0/512.0) * textureColor.g);
     
     highp vec2 texPos2;
     texPos2.x = (quad2.x * 0.125) + 0.5/512.0 + ((0.125 - 1.0/512.0) * textureColor.r);
     texPos2.y = (quad2.y * 0.125) + 0.5/512.0 + ((0.125 - 1.0/512.0) * textureColor.g);
     
     lowp vec4 newColor1 = texture2D(uTextureLookupTable, texPos1);
     lowp vec4 newColor2 = texture2D(uTextureLookupTable, texPos2);
     
     lowp vec4 newColor = mix(newColor1, newColor2, fract(blueColor));
     gl_FragColor = mix(textureColor, vec4(newColor.rgb, textureColor.w), uIntensity);
 }
]]

local _lookuptablePass = nil
local _lookuptable_512Pass = nil
local _lookuptableTex = nil
local _useFixedLUTPass = false
local _pngName

function initParams(context, filter)
	OF_LOGI(TAG, "call initParams")
    filter:insertFloatParam("Intensity", 0.0, 1.0, 1.0)
    filter:insertResParam("LookUpTable", OF_ResType_Image, "whitening.png")
    _pngName = "whitening.png"
	return OF_Result_Success
end

function initRenderer(context, filter)
	OF_LOGI(TAG, "call initRenderer")
    local renderer = context:getRenderer()
    if(renderer == "GeForce GTX 750/PCIe/SSE2" or renderer == "GeForce GTX 1070/PCIe/SSE2") then
        _useFixedLUTPass = true
    end
	_lookuptablePass = context:createCustomShaderPass(vs, fs)
	_lookuptable_512Pass = context:createCustomShaderPass(vs, fs_512)
	return OF_Result_Success
end

function teardownRenderer(context, filter)
	OF_LOGI(TAG, "call teardownRenderer")
	context:destroyCustomShaderPass(_lookuptablePass)
	context:destroyCustomShaderPass(_lookuptable_512Pass)
    context:destroyTexture(_lookuptableTex)
	return OF_Result_Success
end

function applyRGBA(context, filter, frameData, inTex, outTex, debugTex)
    
    local quadRender = context:sharedQuadRender()
    
	context:setViewport(0, 0, outTex.width, outTex.height)
	context:setBlend(false)
	context:bindFBO(outTex)
    if(_lookuptableTex:toOFTexture().width == 64) then
        _lookuptablePass:use()
        _lookuptablePass:setUniformTexture("uTextureIn", 0, inTex.textureID, GL_TEXTURE_2D)
        _lookuptablePass:setUniformTexture("uTextureLookupTable", 1, _lookuptableTex:toOFTexture().textureID, GL_TEXTURE_2D)
        _lookuptablePass:setUniform1f("uIntensity", filter:floatParam("Intensity"))
        _lookuptablePass:setUniform1i("uUseCustomLinearFilter", _useFixedLUTPass)

        quadRender:draw(_lookuptablePass, false)
    elseif(_lookuptableTex:toOFTexture().width == 512) then
        _lookuptable_512Pass:use()
        _lookuptable_512Pass:setUniformTexture("uTextureIn", 0, inTex.textureID, GL_TEXTURE_2D)
        _lookuptable_512Pass:setUniformTexture("uTextureLookupTable", 1, _lookuptableTex:toOFTexture().textureID, GL_TEXTURE_2D)
        _lookuptable_512Pass:setUniform1f("uIntensity", filter:floatParam("Intensity"))

        quadRender:draw(_lookuptable_512Pass, false)
    else
        context:copyTexture(inTex, outTex)
    end

	if debugTex ~= nil then
		context:copyTexture(inTex, debugTex)
	end
	return OF_Result_Success
end

function requiredFrameData(context, game)
	return { OF_RequiredFrameData_None }
end

function onApplyParams(context, filter)
    if(_lookuptableTex == nil) then
        local resPath = filter:resFullPath(filter:resParam("LookUpTable"))
        _lookuptableTex = context:loadTexture(resPath)
        _pngName = filter:resParam("LookUpTable")
    end
    if(_pngName ~= filter:resParam("LookUpTable")) then
        local resPath = filter:resFullPath(filter:resParam("LookUpTable"))
        context:destroyTexture(_lookuptableTex)
        _lookuptableTex = context:loadTexture(resPath)
        _pngName = filter:resParam("LookUpTable")
    end
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