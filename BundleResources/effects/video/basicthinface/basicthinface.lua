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
TAG = "OrangeFilter-BasicThinFaceFilter"
OF_LOGI(TAG, "Call BasicThinFaceFilter lua script!")

local vs = [[
	const int MAXPOINTS = 7;
    attribute vec4 aPosition;
	attribute vec4 aTextureCoord;
	varying vec2 vTexCoord;

    uniform float uAspectRatio;
    uniform vec2 uCenter[MAXPOINTS];
    uniform vec2 uNosePoint;
    uniform float uRadius[MAXPOINTS];
    uniform float uWeight[MAXPOINTS];
    uniform int uType[MAXPOINTS];
    uniform float uIntensity;

    void main()
    {
        gl_Position = aPosition;
        vec2 uv = aTextureCoord.xy;
        vec2 textureCoordinateToUse = vec2(uv.x * uAspectRatio, uv.y);
        
        for(int i = 0; i < MAXPOINTS; ++i)
        {
            float e1 = ( textureCoordinateToUse.x - uCenter[i].x ) / ( uRadius[i]);
            float e2 = ( textureCoordinateToUse.y - uCenter[i].y ) / ( uRadius[i]);
            float d  = (e1 * e1) + (e2 * e2);
            if (d < 1.0)
            {
                if(uType[i] == 1)
                {
                    vec2 moveVec = vec2(0.0, 0.0);
                    float weight = sqrt(d);
                    weight = 1.0 - weight;
                    moveVec = uNosePoint - textureCoordinateToUse;
                    textureCoordinateToUse = textureCoordinateToUse - moveVec*uWeight[i]*weight*uIntensity;
                }
                else if(uType[i] == 2)
                {
                    vec2 moveVec = vec2(0.0, 0.0);
                    float weight = sqrt(d);
                    weight = 1.0 - weight;
                    moveVec = uCenter[i] - textureCoordinateToUse;
                    textureCoordinateToUse = textureCoordinateToUse + moveVec*uWeight[i]*weight*uIntensity;
                }
            }
        }
        
        vTexCoord = vec2(textureCoordinateToUse.x /uAspectRatio, textureCoordinateToUse.y);
    }
]]

local fs = [[
	precision mediump float;
	varying vec2 vTexCoord;
	uniform sampler2D uInputImageTexture;

	void main()
	{
	    vec4 color = texture2D(uInputImageTexture, vTexCoord);
	    gl_FragColor = color;
	}
]]

local MAXPOINTS = 7

local _meshCol = 70
local _meshRow = 70

local _texCoords = nil
local _positions = nil
local _indices = nil
local _mesh2dRender = nil -- _faceLiftingRender

local _type = { 1, 1, 1, 1, 1, 2, 2} -- int _type[MAXPOINTS]
local _intensity = 0.7
local _eyeWeight = 0.0
local _chinWeight = 0.0

local _basicthinface_pass = nil

--face lost strategy
local _isFaceLost = false
local _lastFaceArray = OF_FrameData.new()
local _lastFaceCount = 0
local _smoothFrme = 20
local _smoothFrmeCount = 0

local _setByMessage = false

function eyeBallsLength(lefteyePoint, righteyePoint)
	local eyeVector = lefteyePoint - righteyePoint
	return eyeVector:length()
end

function initParams(context, filter)
	OF_LOGI(TAG, "call initParams")

	filter:insertFloatParam("MeshCol", 5.0, 150.0, 70.0)
    filter:insertFloatParam("Intensity", 0.0, 1.0, 0.4)
    filter:insertFloatParam("Chin", 0.8, 1.0, 0.95)
    filter:insertFloatParam("BigEye", 0.0, 1.0, 0.5)

	return OF_Result_Success
end

function onApplyParams(context, filter)
	if not _setByMessage then
		_intensity = filter:floatParam("Intensity")*1.0
	end
    local _eyeparam = filter:floatParam("BigEye")*1.0
    _eyeWeight = -0.4*_eyeparam*_eyeparam + 0.9*_eyeparam
    local _chinparam = filter:floatParam("Chin")*1.0
    _chinWeight = -1.67*_chinparam*_chinparam + 2.25*_chinparam - 0.58;
	return OF_Result_Success
end

function initRenderer(context, filter)
	OF_LOGI(TAG, "call initRenderer")
    _basicthinface_pass = context:createCustomShaderPass(vs, fs)

    if _basicthinface_pass == nil then
        OF_LOGE(TAG, "BasicThinFaceFilter programe is nil!")
    end

	updateMesh(_meshRow, _meshCol)

	return OF_Result_Success
end

function teardownRenderer(context, filter)
	OF_LOGI(TAG, "call teardownRenderer")
    if(_basicthinface_pass) then
        context:destroyCustomShaderPass(_basicthinface_pass)
        _basicthinface_pass = nil
    end

	return OF_Result_Success
end

function updateMesh(meshRow, meshCol)
	_meshRow = meshRow
	_meshCol = meshCol

	_texCoords = nil
	_positions = nil
	_indices   = nil
	_mesh2dRender = nil

	local pointCount = _meshRow * _meshCol
	_texCoords = FloatArray.new(pointCount * 2)
	_positions = FloatArray.new(pointCount * 2)
    local texCoordsTable = {}
    local positionsTable = {}
	local k = 1
	for i = 0 , _meshRow - 1 do
		for j = 0 , _meshCol - 1 do
            texCoordsTable[2 * k -1] = j / (_meshCol - 1)
            texCoordsTable[2 * k ] = i / (_meshRow - 1)
			positionsTable[2 * k - 1] = j / (_meshCol - 1) * 2.0 - 1.0
			positionsTable[2 * k] = i / (_meshRow - 1) * 2.0 - 1.0
			k = k + 1
		end
	end
	_texCoords:copyFromTable(texCoordsTable) -- use copeFromTable OF version must be  higher than 4.5.0
	_positions:copyFromTable(positionsTable) -- use copeFromTable OF version must be  higher than 4.5.0

	local indicesCount = (_meshRow - 1) * (_meshCol - 1) * 2 * 3
	_indices = IntArray.new(indicesCount)
    local indicesTable = {}
	k = 1
	for i = 0, _meshRow - 2 do
		for j = 0, _meshCol - 2 do
            indicesTable[k] = i * _meshCol + j
			k = k + 1
            indicesTable[k] = i * _meshCol + j + 1
			k = k + 1
            indicesTable[k] = (i + 1) * _meshCol + j
			k = k + 1
            indicesTable[k] = i * _meshCol + j + 1
			k = k + 1
            indicesTable[k] = (i + 1) * _meshCol + j + 1
			k = k + 1
            indicesTable[k] = (i + 1) * _meshCol + j
			k = k + 1
		end
	end
    _indices:copyFromTable(indicesTable) -- use copeFromTable OF version must be  higher than 4.5.0

	_mesh2dRender = Mesh2dRender.new(_positions, _texCoords, pointCount, _indices, indicesCount)
	_mesh2dRender:setWireframe(false, false)
end

function fillArrayData(array, t)
	local offset = 0
	-- print("fillArrayData------")
	local cnt = #t
	if t[0] then
		cnt = cnt + 1
		offset = -1
	end
	-- print(cnt, offset)
	for i=1,cnt do
		array:set(i-1, t[i+offset])
		-- print(i-1, t[i])
	end
end

local _facePoint = nil
local _centerArray = FloatArray.new(MAXPOINTS * 2)
local _radiusArray = FloatArray.new(MAXPOINTS)
local _weightArray = FloatArray.new(MAXPOINTS)
local _typeArray = IntArray.new(MAXPOINTS)

function applySingleFace(context, filter, faceIndex, faceFrameData, input, output, debug)
    local width = output.width
	local height = output.height

    local flmPoints = faceFrameData.facePoints

	local function GETPOINT_X(x)
		return flmPoints[x*2 + 1]
	end
	local function GETPOINT_Y(y)
		return flmPoints[y*2 + 2]
	end

    local whRatio = width / height
    if _facePoint == nil then
		_facePoint = {}
		for i=0, 105 do
			_facePoint[i] = Vec2f.new()
		end
    end

    for i=0, 105 do
        _facePoint[i].x = GETPOINT_X(i)*whRatio
        _facePoint[i].y =  GETPOINT_Y(i)
    end

    local standLength = eyeBallsLength(_facePoint[74], _facePoint[77])

    context:bindFBO(output)
	context:setViewport(0, 0, width, height)
	context:setBlend(false)
    local _centerPoints = {_facePoint[16].x, _facePoint[16].y, _facePoint[10].x, _facePoint[10].y, _facePoint[22].x, _facePoint[22].y, _facePoint[4].x, _facePoint[4].y, _facePoint[28].x, _facePoint[28].y, _facePoint[74].x, _facePoint[74].y, _facePoint[77].x, _facePoint[77].y}
    local _radius = {standLength*1.1, standLength*1.2, standLength*1.2, standLength, standLength, standLength*0.25, standLength*0.25}
    local _weight = {_chinWeight, 0.1, 0.1, 0.05, 0.05, _eyeWeight, _eyeWeight}
    fillArrayData(_centerArray, _centerPoints)
    fillArrayData(_radiusArray, _radius)
    fillArrayData(_weightArray, _weight)
    fillArrayData(_typeArray, _type)

    if (_basicthinface_pass) then
        context:setClearColor(0.0, 0.0, 0.0, 1.0)
	    context:clearColorBuffer()

        _basicthinface_pass:use()
        _basicthinface_pass:setUniformTexture("uInputImageTexture", 0, input.textureID, TEXTURE_2D)
        _basicthinface_pass:setUniform1f("uAspectRatio", width / height)
        _basicthinface_pass:setUniform1fv("uRadius", MAXPOINTS, _radiusArray)
        _basicthinface_pass:setUniform1fv("uWeight", MAXPOINTS, _weightArray)
        _basicthinface_pass:setUniform2fv("uCenter", MAXPOINTS, _centerArray)
        _basicthinface_pass:setUniform1iv("uType", MAXPOINTS, _typeArray)
        _basicthinface_pass:setUniform2f("uNosePoint", _facePoint[46].x, _facePoint[46].y)
        _basicthinface_pass:setUniform1f("uIntensity", 0.65*_intensity)

        _mesh2dRender:draw(_basicthinface_pass, false)
    else
        context:copyTexture(input, output)
        OF_LOGE(TAG, "_basicthinface_pass is nil!")
    end
end

function applyRGBA(context, filter, frameData, inTex, outTex, debugTex)
	local width = outTex.width
	local height = outTex.height
    local faceCount = frameData.faceFrameDataArr.faceCount
    if((faceCount <= 0 and _lastFaceCount == 0) or _intensity < 0.000001) then
        context:copyTexture(inTex, outTex)
		return OF_Result_Success
	elseif(faceCount <= 0 and _lastFaceCount ~= 0) then
		_isFaceLost = true
    else
        _isFaceLost = false
		_smoothFrmeCount = 0
    end

	if(not _isFaceLost)then
		_lastFaceCount = faceCount
		OF_FrameData.CopyFrameData(_lastFaceArray, frameData)
	end

    local cols = math.floor(filter:floatParam("MeshCol")*1.0)
    local rows = math.floor(cols *1.0 * height / width)
    if(_meshCol ~= cols or rows ~= _meshRow)then
        updateMesh(rows, cols)
        _meshCol = cols
        _meshRow = rows
    end

	local tempInTex = context:getTexture(width, height)
	if(not _isFaceLost) then
		local isOddCount = ((faceCount % 2) ~= 0)
		local tempOFTex = tempInTex:toOFTexture()

		local tem_inTex = outTex
		local tem_outTex = outTex
		if(isOddCount) then
			tem_inTex = tempOFTex
		else
			tem_outTex = tempOFTex
		end
		applySingleFace(context, filter, 1, frameData.faceFrameDataArr.faceItemArr[1], inTex, tem_outTex, debugTex)

		for i=2 ,frameData.faceFrameDataArr.faceCount do
			local faceData = frameData.faceFrameDataArr.faceItemArr[i]
			tem_inTex, tem_outTex = tem_outTex, tem_inTex
			applySingleFace(context, filter, i, faceData, tem_inTex, tem_outTex, debugTex)
		end
    else
		if(_smoothFrmeCount < _smoothFrme) then
			local isOddCount = ((_lastFaceCount % 2) ~= 0)
			local tempOFTex = tempInTex:toOFTexture()

			local tem_inTex = outTex
			local tem_outTex = outTex
			if(isOddCount) then
				tem_inTex = tempOFTex
			else
				tem_outTex = tempOFTex
			end
			applySingleFace(context, filter, 1, _lastFaceArray.faceFrameDataArr.faceItemArr[1], inTex, tem_outTex, debugTex)

			for i=2 ,_lastFaceCount do
				local faceData = _lastFaceArray.faceFrameDataArr.faceItemArr[i]
				tem_inTex, tem_outTex = tem_outTex, tem_inTex
				applySingleFace(context, filter, i, faceData, tem_inTex, tem_outTex, debugTex)
			end
            _smoothFrmeCount = _smoothFrmeCount + 1
		else
			context:copyTexture(inTex, outTex)
		end
	end
    context:releaseTexture(tempInTex)

	return OF_Result_Success
end

function requiredFrameData(context, game)
	return { OF_RequiredFrameData_FaceLandmarker }
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
	OF_LOGI("basicthinface", string.format("call onReceiveMessage %s", msg))
	local tab = Json.JsonToTable(msg)
	if tab.thinface then
		_intensity = tab.thinface / 100
		_setByMessage = true
	end
	return ""
end
