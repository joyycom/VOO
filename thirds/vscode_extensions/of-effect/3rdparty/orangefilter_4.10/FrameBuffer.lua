--* This Document is AutoGenerate by OrangeFilter, Don't Change it! *

---@meta

---
---[3.3]frame buffer[parent:BaseObject#baseobject]
---
---@class FrameBuffer
FrameBuffer = {}

---
---[3.3]get gl frame buffer id
---
--- @return int
--- @nodiscard
function FrameBuffer:glID() end
---
---[3.3]bind frame buffer
---
--- @return bool
--- @nodiscard
function FrameBuffer:bind() end
---
---[3.3]attach texture
---
--- @param arg0 int
--- @param arg1 int
--- @param arg2 GL_FRAMEBUFFER_ATTACHMENT#gl_framebuffer_attachment
--- @param arg3 int
--- @param arg4 int
--- @nodiscard
function FrameBuffer:attachTexture(arg0, arg1, arg2, arg3, arg4) end
---
---[3.3]attach render buffer
---
--- @param arg0 int
--- @param arg1 GL_FRAMEBUFFER_ATTACHMENT#gl_framebuffer_attachment
--- @nodiscard
function FrameBuffer:attachRenderBuffer(arg0, arg1) end
---
---[3.3]unattach framebuffer
---
--- @param arg0 GL_FRAMEBUFFER_ATTACHMENT#gl_framebuffer_attachment
--- @nodiscard
function FrameBuffer:unattach(arg0) end
---
---[3.3]unattach all attachment
---
--- @nodiscard
function FrameBuffer:unattachAll() end
---
---[3.3]get attached type
---
--- @param arg0 GL_FRAMEBUFFER_ATTACHMENT#gl_framebuffer_attachment
--- @return int
--- @nodiscard
function FrameBuffer:attachedType(arg0) end
---
---[3.3]get attached object id
---
--- @param arg0 GL_FRAMEBUFFER_ATTACHMENT#gl_framebuffer_attachment
--- @return int
--- @nodiscard
function FrameBuffer:attachedID(arg0) end
---
---[3.3]get attached texture mip level
---
--- @param arg0 GL_FRAMEBUFFER_ATTACHMENT#gl_framebuffer_attachment
--- @return int
--- @nodiscard
function FrameBuffer:attachedMipLevel(arg0) end
---
---[3.3]get attached cubemap face
---
--- @param arg0 GL_FRAMEBUFFER_ATTACHMENT#gl_framebuffer_attachment
--- @return GL_CUBE_MAP_FACE#gl_cube_map_face
--- @nodiscard
function FrameBuffer:attachedCubeFace(arg0) end

---
---[3.3]get max color attachments
---
--- @return int
--- @nodiscard
function FrameBuffer.maxColorAttachments() end
---
---[3.3]disable all framebuffers
---
--- @nodiscard
function FrameBuffer.disable() end

return FrameBuffer
