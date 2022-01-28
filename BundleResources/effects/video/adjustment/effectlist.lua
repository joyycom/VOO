local EffectList = {
    effectList = {},
    effectPaths = {}
}

function EffectList:init(context)
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
        context:destroyEffect(self.effectList[i])
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
            local effectId = context:createEffectFromFile(effectPath, effectDir)
            if effectId > 0 then
                table.insert(self.effectList, effectId)
            else
                OF_LOGE("EffectList", string.format("Create effect failed. %s", effectPath))
            end
        end
        self.effectPaths = paths
    end
end

function EffectList:apply(context, frameData, inTex, outTex, debugTex, filterTimestamp)
    local width = inTex.width
    local height = inTex.height
    local cachedTexture = context:getTexture(width, height)
    local tempTex = cachedTexture:toOFTexture()
    local idList = self.effectList

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
        context:seekEffectAnimation(idList[1], t)
        context:applyRGBA(idList[i], frameData, pIn, pOut, debugTex)
    end

    context:releaseTexture(cachedTexture)
end

function EffectList:sendMessage(context, idx, msg)
    context:sendMessage(self.effectList[idx], msg)
end

return EffectList