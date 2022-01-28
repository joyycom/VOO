TAG = "OF-Adjustment"
OF_LOGI(TAG, "Call Adjustment lua script!")

local EffectList = require "effectlist"
local Filter = {
	brightness = 0,
	contrast = 0,
	saturation = 0,
	sharpen = 0,
	highlight = 0,
	shadow = 0,
	fade = 0,
	temperature = 0,
	tone = 0
}

function Filter:initParams(context, filter)
	OF_LOGI(TAG, "call initParams")
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
	return OF_Result_Success
end

function Filter:onApplyParams(context, filter)
	OF_LOGI(TAG, "call onApplyParams")

	-- effect list
    local effectListTable = filter:resArrParam("EffectList")
	local paths = {}
	for i = 1, #effectListTable do
		table.insert(paths, filter:resFullPath(effectListTable[i]))
	end
	EffectList:setEffectPaths(context, paths)

	self.brightness = filter:intParam("Brightness")
	self.contrast = filter:intParam("Contrast")
	self.saturation = filter:intParam("Saturation")
	self.sharpen = filter:intParam("Sharpen")
	self.highlight = filter:intParam("Highlight")
	self.shadow = filter:intParam("Shadow")
	self.fade = filter:intParam("Fade")
	self.temperature = filter:intParam("Temperature")
	self.tone = filter:intParam("Tone")

	self.sendMessage(self, context, filter)

	return OF_Result_Success
end

function Filter:sendMessage(context, filter)
	if EffectList:isEmpty() then
		local effectListTable = filter:resArrParam("EffectList")
		local paths = {}
		for i = 1, #effectListTable do
			table.insert(paths, filter:resFullPath(effectListTable[i]))
		end
		EffectList:setEffectPaths(context, paths)
	end
	EffectList:sendMessage(context, 1, Json.TableToJson({ intensity = self.brightness / 50 }))
	EffectList:sendMessage(context, 2, Json.TableToJson({ intensity = self.contrast / 50 }))
	EffectList:sendMessage(context, 3, Json.TableToJson({ intensity = self.saturation / 50 }))
	EffectList:sendMessage(context, 4, Json.TableToJson({ intensity = self.sharpen / 100 }))
	EffectList:sendMessage(context, 5, Json.TableToJson({ intensity = self.highlight / 100 }))
	EffectList:sendMessage(context, 6, Json.TableToJson({ intensity = self.shadow / 100 }))
	EffectList:sendMessage(context, 7, Json.TableToJson({ intensity = self.fade / 100 }))
	EffectList:sendMessage(context, 8, Json.TableToJson({ intensity = self.temperature / 50 }))
	EffectList:sendMessage(context, 9, Json.TableToJson({ intensity = self.tone / 50 }))
end

function Filter:initRenderer(context, filter)
	OF_LOGI(TAG, "call initRenderer")
	EffectList:init(context)
	return OF_Result_Success
end

function Filter:teardownRenderer(context, filter)
	OF_LOGI(TAG, "call teardownRenderer")
	EffectList:clear(context)
	return OF_Result_Success
end

function Filter:applyRGBA(context, filter, frameData, inTex, outTex, debugTex)
	if not EffectList:isEmpty() then
		EffectList:apply(context, frameData, inTex, outTex, debugTex, filter:filterTimestamp())
	else
		context:copyTexture(inTex, outTex)
	end

	if debugTex ~= nil then
		context:copyTexture(inTex, debugTex)
	end
	return OF_Result_Success
end

function Filter:requiredFrameData(context, game)
	return { OF_RequiredFrameData_None }
end

function Filter:readObject(context, filter, archiveIn)
	OF_LOGI(TAG, "call readObject")
	return OF_Result_Success
end

function Filter:writeObject(context, filter, archiveOut)
	OF_LOGI(TAG, "call writeObject")
	return OF_Result_Success
end

function Filter:onReceiveMessage(context, filter, msg)
	OF_LOGI(TAG, string.format("call onReceiveMessage %s", msg))
	local tab = Json.JsonToTable(msg)
	if tab.brightness  then self.brightness = tab.brightness  end
	if tab.contrast    then self.contrast   = tab.contrast    end
	if tab.saturation  then self.saturation = tab.saturation  end
	if tab.sharpen     then self.sharpen    = tab.sharpen     end
	if tab.highlight   then self.highlight  = tab.highlight   end
	if tab.shadow      then self.shadow     = tab.shadow      end
	if tab.fade        then self.fade       = tab.fade        end
	if tab.temperature then self.temperature= tab.temperature end
	if tab.tone        then self.tone       = tab.tone        end
	self.sendMessage(self, context, filter)
	return ""
end

return Filter
