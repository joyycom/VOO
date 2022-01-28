local EffectList = {
    effectList = {},
    effectPaths = {},
    params = {
        beauty      = 0, 
		fade        = 0,
        thinface    = 0,
        brightness  = 0,
		contrast    = 0,
		saturation  = 0,
		sharpen     = 0,
		highlight   = 0,
		shadow      = 0,
		temperature = 0,
		tone        = 0, 
    }
}

function EffectList:initParams(context, filter)
	filter:insertIntParam("Beauty", 0, 100, 0)
	filter:insertIntParam("ThinFace", 0, 100, 0)
	filter:insertIntParam("Brightness", -50, 50, 0)
	filter:insertIntParam("Contrast", -50, 50, 0)
	filter:insertIntParam("Saturation", -50, 50, 0)
	filter:insertIntParam("Sharpen", 0, 100, 0)
	filter:insertIntParam("Highlight", 0, 100, 0)
	filter:insertIntParam("Shadow", 0, 100, 0)
	filter:insertIntParam("Fade", 0, 100, 0)
	filter:insertIntParam("Temperature", -50, 50, 0)
	filter:insertIntParam("Tone", -50, 50, 0)
    
	filter:insertResArrParam("EffectList", OF_ResType_Effect)
end

function EffectList:onApplyParams(context, filter)
    -- effect list
    local effectListTable = filter:resArrParam("EffectList")
    local paths = {}
    for i = 1, #effectListTable do
        table.insert(paths, filter:resFullPath(effectListTable[i]))
    end
    self:setEffectPaths(context, paths)

	self.params.beauty = filter:intParam("Beauty")
	self.params.thinface = filter:intParam("ThinFace")
    self.params.brightness  = filter:intParam("Brightness")
    self.params.contrast    = filter:intParam("Contrast")
    self.params.saturation  = filter:intParam("Saturation")
    self.params.sharpen     = filter:intParam("Sharpen")
    self.params.highlight   = filter:intParam("Highlight")
    self.params.shadow      = filter:intParam("Shadow")
    self.params.fade        = filter:intParam("Fade")
    self.params.temperature = filter:intParam("Temperature")
    self.params.tone        = filter:intParam("Tone")

    self:setEnabled(1,  self.params.beauty > 0)
	self:setEnabled(2,  self.params.fade        ~= 0)
	self:setEnabled(3,  self.params.tone        ~= 0)
	self:setEnabled(4,  self.params.sharpen     ~= 0)
	self:setEnabled(5,  self.params.shadow      ~= 0)
	self:setEnabled(6,  self.params.thinface > 0)
    self:setEnabled(7,  self.params.contrast    ~= 0)
	self:setEnabled(8,  self.params.highlight   ~= 0)
	self:setEnabled(9,  self.params.brightness  ~= 0)
	self:setEnabled(10, self.params.saturation  ~= 0)
	self:setEnabled(11, self.params.temperature ~= 0)
	self:sendMessage(context, 1,   Json.TableToJson({ beauty = self.params.beauty }))
	self:sendMessage(context, 2,   Json.TableToJson({ intensity = self.params.fade / 100 }))
	self:sendMessage(context, 3,   Json.TableToJson({ intensity = self.params.tone / 50 }))
	self:sendMessage(context, 4,   Json.TableToJson({ intensity = self.params.sharpen / 100 }))
	self:sendMessage(context, 5,   Json.TableToJson({ intensity = self.params.shadow / 100 }))
	self:sendMessage(context, 6,   Json.TableToJson({ thinface = self.params.thinface }))
	self:sendMessage(context, 7,   Json.TableToJson({ intensity = self.params.contrast / 50 }))
	self:sendMessage(context, 8,   Json.TableToJson({ intensity = self.params.highlight / 100 }))
	self:sendMessage(context, 9,   Json.TableToJson({ intensity = self.params.brightness / 50 }))
	self:sendMessage(context, 10,  Json.TableToJson({ intensity = self.params.saturation / 50 }))
	self:sendMessage(context, 11,  Json.TableToJson({ intensity = self.params.temperature / 50 }))
end


function EffectList:isEmpty()
    return #self.effectList == 0
end

function EffectList:isSamePaths(paths)
    if #paths ~= #self.effectPaths then
        return false
    end

    for i = 1, #paths do
        if paths[i] ~= self.effectPaths[i] then
            return false
        end
    end
    return true
end

function EffectList:clear(context)
    local cnt = #self.effectList
    for i = 1, cnt do
        if self.effectList[i].inited then
            context:destroyEffect(self.effectList[i].id)
        end
    end
    self.effectList = {}
end

function EffectList:setEffectPaths(context, paths)
    if not self.isSamePaths(self, paths) then
        self.clear(self, context)
        for i = 1, #paths do
            local effectPath = paths[i]
            effectPath = string.gsub(effectPath, "\\", "/")
            local effectDir = string.match(effectPath, ".*/")
            table.insert(self.effectList, { id = 0,
                                            enabled = false, 
                                            inited = false,
                                            effectDir = effectDir,
                                            effectPath = effectPath,
                                            lastMsg = nil })
            -- OF_LOGE("EffectList", string.format("EffectList effectDir = %s effectPath = %s", effectDir, effectPath))
        end
        self.effectPaths = paths
    end
end

function EffectList:applyBatch(context, idList, frameData, inTex, outTex, debugTex, filterTimestamp)
    local width = inTex.width
    local height = inTex.height
    local cachedTexture = context:getTexture(width, height)
    local tempTex = cachedTexture:toOFTexture()

    local isOddCount = false
    if (#idList % 2) ~= 0 then
        isOddCount = true
    end

    local pIn, pOut = outTex, tempTex
    if isOddCount == true then
        pIn, pOut = tempTex, outTex
    end

    local timestamp = filterTimestamp * 1000
    local t = i64.new(timestamp)
    context:seekEffectAnimation(idList[1], t)

    context:applyRGBA(idList[1], frameData, inTex, pOut, debugTex)

    for i = 2, #idList do
        pIn, pOut = pOut, pIn
        context:seekEffectAnimation(idList[i], t)
        context:applyRGBA(idList[i], frameData, pIn, pOut, debugTex)
    end

    context:releaseTexture(cachedTexture)
end

function EffectList:dump()
    for i = 1, #self.effectList do
        print(i, self.effectList[i].id, self.effectList[i].enabled)
    end
end

function EffectList:apply(context, frameData, inTex, outTex, debugTex, filterTimestamp)
    --self.dump(self)
    local ids = {}
    for i = 1, #self.effectList do
        if self.effectList[i].enabled then
            if self.effectList[i].inited == false then
                local effectId = context:createEffectFromFile(self.effectList[i].effectPath, self.effectList[i].effectDir)
                -- OF_LOGE("EffectList", string.format("Create effect effectDir = %s effectPath = %s . effectId = %d", self.effectList[i].effectDir, self.effectList[i].effectPath, effectId))
                if effectId > 0 then
                    self.effectList[i].inited = true
                    self.effectList[i].id = effectId
                    if self.effectList[i].lastMsg ~= nil then
                        context:sendMessage(self.effectList[i].id, self.effectList[i].lastMsg)
                        self.effectList[i].lastMsg = nil
                    end
                else
                    OF_LOGE("EffectList", string.format("Create effect failed. %s", effectPath))
                end
            end

            if self.effectList[i].inited then
                table.insert(ids, self.effectList[i].id)
            end
        end
    end

    if #ids > 0 then
        self.applyBatch(self, context, ids, frameData, inTex, outTex, debugTex, filterTimestamp)
    else
        context:copyTexture(inTex, outTex)
    end
end

function EffectList:setEnabled(idx, enabled)
    if idx <= #self.effectList then
        self.effectList[idx].enabled = enabled
    end
end

function EffectList:sendMessage(context, idx, msg)
    if idx <= #self.effectList then
        if self.effectList[idx].inited == false then
            self.effectList[idx].lastMsg = msg
        else
            context:sendMessage(self.effectList[idx].id, msg)
        end
    end
end

return EffectList