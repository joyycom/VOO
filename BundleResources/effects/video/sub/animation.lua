local Config = require "sub.config"
local TAG = "Animation Script"
local Animation = {
    enter_animation = nil, enter_duration = 1,
    exit_animation = nil, exit_duration = 1,
    loop_animation = nil, loop_duration = 1,
    params = {
        alpha = 1.0,
        position = { 0, 0, 0 },
        scale = { 1.0, 1.0, 1.0 },
        rotation = { 0.0, 0.0, 0.0 },
        localPosition = { 0, 0, 0 },
        localScale = { 1.0, 1.0, 1.0 },
        localRotation = { 0.0, 0.0, 0.0 },

        renderToRT = nil,
    },
}

function Animation:initParams(context, filter)
	filter:insertEnumParam("EnterAnim", 0, Config.enter_animation)
	filter:insertStringParam("EnterAnimDir", "")
	filter:insertFloatParam("EnterAnimDuration", 0, 10, 1)

	filter:insertEnumParam("ExitAnim", 0, Config.exit_animation)
	filter:insertStringParam("ExitAnimDir", "")
	filter:insertFloatParam("ExitAnimDuration", 0, 10, 1)

	filter:insertEnumParam("LoopAnim", 0, Config.loop_animation)
	filter:insertStringParam("LoopAnimDir", "")
	filter:insertFloatParam("LoopAnimDuration", 0, 10, 1)
end

function Animation:loadFromFilter(context, filter)
    OF_LOGI(TAG, "call Animation onApplyParams")

    local idx =  filter:enumParam("EnterAnim") + 1
	local name = Config.enter_animation[idx]
	local effect_dir = filter:stringParam("EnterAnimDir")
	local enterAnimPath = filter:filterDir() .. "/../enter_animation/" .. name .. "/" .. name .. ".lua"
	if string.len(effect_dir) > 0 then
		local s = context:loadTextFromFile(effect_dir .. "/config.json")
		local tab = Json.JsonToTable(s)
		enterAnimPath = effect_dir .. "/" .. tab.script
	end
	if idx > 1 or string.len(effect_dir) > 0 then -- not none
		if self.enter_animation then
            self.enter_animation:clear(context)
        end
		self.enter_animation = dofile(enterAnimPath)
		self.enter_animation:init(context)
		self.enter_duration = filter:floatParam("EnterAnimDuration") * 1000
		self.enter_animation:setDuration(nil, self.enter_duration)
	else
		self.enter_animation = nil
	end

	idx =  filter:enumParam("ExitAnim") + 1
	name = Config.exit_animation[idx]
	effect_dir = filter:stringParam("ExitAnimDir")
	local exitAnimPath = filter:filterDir() .. "/../exit_animation/" .. name .. "/" .. name .. ".lua"
	if string.len(effect_dir) > 0 then
		local s = context:loadTextFromFile(effect_dir .. "/config.json")
		local tab = Json.JsonToTable(s)
		exitAnimPath = effect_dir .. "/" .. tab.script
	end
	if idx > 1 or string.len(effect_dir) > 0 then -- not none
		if self.exit_animation then
            self.exit_animation:clear(context)
        end
		self.exit_animation = dofile(exitAnimPath)
		self.exit_animation:init(context)
		self.exit_duration = filter:floatParam("ExitAnimDuration") * 1000
		self.exit_animation:setDuration(nil, self.exit_duration)
	else
		self.exit_animation = nil
	end

	idx =  filter:enumParam("LoopAnim") + 1
	name = Config.loop_animation[idx]
	effect_dir = filter:stringParam("LoopAnimDir")
	local loopAnimPath = filter:filterDir() .. "/../loop_animation/" .. name .. "/" .. name .. ".lua"
	if string.len(effect_dir) > 0 then
		local s = context:loadTextFromFile(effect_dir .. "/config.json")
		local tab = Json.JsonToTable(s)
		loopAnimPath = effect_dir .. "/" .. tab.script
	end
	if idx > 1 or string.len(effect_dir) > 0 then -- not none
		if self.loop_animation then
            self.loop_animation:clear(context)
        end
		self.loop_animation = dofile(loopAnimPath)
		self.loop_animation:init(context)
		self.loop_duration = filter:floatParam("LoopAnimDuration") * 1000
		self.loop_animation:setDuration(nil, self.loop_duration)
	else
		self.loop_animation = nil
	end
end

function Animation:loadFromMsg(context, evt)
    if evt.id == 2 then
        OF_LOGI(TAG, string.format("%d, %s", evt.id, evt.effect_dir))
        if string.len(evt.effect_dir) > 0 then
            if self.enter_animation then
                self.enter_animation:clear(context)
            end
            local s = context:loadTextFromFile(evt.effect_dir .. "/config.json")
            local tab = Json.JsonToTable(s)
            self.enter_animation = dofile(evt.effect_dir .. "/" .. tab.script)
            self.enter_animation:init(context)
            self.enter_duration = evt.duration * 1000
            self.enter_animation:setDuration(nil, self.enter_duration)
        else
            self.enter_animation = nil
        end
    elseif evt.id == 3 then
        OF_LOGI(TAG, string.format("%d, %s", evt.id, evt.effect_dir))
        if string.len(evt.effect_dir) > 0 then
            if self.exit_animation then
                self.exit_animation:clear(context)
            end
            local s = context:loadTextFromFile(evt.effect_dir .. "/config.json")
            local tab = Json.JsonToTable(s)
            self.exit_animation = dofile(evt.effect_dir .. "/" .. tab.script)
            self.exit_animation:init(context)
            self.exit_duration = evt.duration * 1000
            self.exit_animation:setDuration(nil, self.exit_duration)
        else
            self.exit_animation = nil
        end
    elseif evt.id == 4 then
        OF_LOGI(TAG, string.format("%d, %s", evt.id, evt.effect_dir))
        if string.len(evt.effect_dir) > 0 then
            if self.loop_animation then
                self.loop_animation:clear(context)
            end
            local s = context:loadTextFromFile(evt.effect_dir .. "/config.json")
            local tab = Json.JsonToTable(s)
            self.loop_animation = dofile(evt.effect_dir .. "/" .. tab.script)
            self.loop_animation:init(context)
            self.loop_duration = evt.duration * 1000
            self.loop_animation:setDuration(nil, self.loop_duration)
        else
            self.loop_animation = nil
        end
    end
end

function Animation:seek(Filter, filter)
    self.params = {
		alpha = 1.0,
		position = { 0, 0, 0 },
		scale = { 1.0, 1.0, 1.0 },
		rotation = { 0.0, 0.0, 0.0 },
		localPosition = { 0, 0, 0},
		localScale = { 1.0, 1.0, 1.0 },
		localRotation = { 0.0, 0.0, 0.0 },
		renderToRT = nil
	}
    local timestamp = filter:filterTimestamp() * 1000
	if self.loop_animation then
		self.loop_animation:seek(Filter, timestamp)
		self.loop_animation:apply(Filter)
		return
	else
		if self.enter_animation and timestamp < self.enter_duration then
			self.enter_animation:seek(Filter, timestamp)
			self.enter_animation:apply(Filter)
			return
		end

		local exitTime = filter:duration() * 1000 - self.exit_duration
		if self.exit_animation and timestamp > exitTime then
			self.exit_animation:seek(Filter, timestamp - exitTime)
			self.exit_animation:apply(Filter)
			return
		end
	end
	return
end

function Animation:apply(Filter, filter, texTemp)
    local timestamp = filter:filterTimestamp() * 1000
    if self.loop_animation then
		self.loop_animation:applyEffect(Filter, texTemp)
	elseif self.enter_animation and timestamp < self.enter_duration then
		self.enter_animation:applyEffect(Filter, texTemp)
	else
		local exitTime = filter:duration() * 1000 - self.exit_duration
		if self.exit_animation and timestamp > exitTime then
			self.exit_animation:applyEffect(Filter, texTemp)
		end
	end
end

function Animation:clear(context)
    if self.enter_animation then
        self.enter_animation:clear(context)
    end

    if self.exit_animation then
        self.exit_animation:clear(context)
    end

    if self.loop_animation then
        self.loop_animation:clear(context)
    end
end

return Animation