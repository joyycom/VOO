--* This Document is AutoGenerate by OrangeFilter, Don't Change it! *

---@meta

---
---[3.6]animation system[parent:Component#component]
---
---@class Animation
Animation = {}

---
---[3.6]get clip count
---
--- @return int
--- @nodiscard
function Animation:getClipCount() end
---
---[3.6]get clip name
---
--- @param arg0 int
--- @return string
--- @nodiscard
function Animation:getClipName(arg0) end
---
---[3.6]has some clip exist
---
--- @param arg0 string
--- @return bool
--- @nodiscard
function Animation:hasClip(arg0) end
---
---[3.6]get clip by name
---
--- @param arg0 string
--- @return AnimationClip
--- @nodiscard
function Animation:getClip(arg0) end
---
---[3.7]set play mode
---
--- @param arg0 OF_AnimationPlayMode#of_animationplaymode
--- @nodiscard
function Animation:setPlayMode(arg0) end
---
---[3.6]play clip
---
--- @param arg0 string
--- @nodiscard
function Animation:play(arg0) end
---
---[3.6]stop clip
---
--- @param arg0 string
--- @nodiscard
function Animation:stop(arg0) end
---
---[3.6]pause clip
---
--- @param arg0 string
--- @nodiscard
function Animation:pause(arg0) end
---
---[4.1]cross fade clip
---
--- @param arg0 string
--- @param arg1 float
--- @nodiscard
function Animation:crossFade(arg0, arg1) end
---
---[4.4]set playing time
---
--- @param arg0 float
--- @nodiscard
function Animation:setTime(arg0) end
---
---[4.1]get play state
---
--- @return OF_AnimationPlayMode
--- @nodiscard
function Animation:getPlayState() end

---
---[4.1]transfer clip owner
---
--- @param arg0 string
--- @param arg1 Animation#animation
--- @param arg2 Animation#animation
--- @return bool
--- @nodiscard
function Animation.MoveClip(arg0, arg1, arg2) end

return Animation
