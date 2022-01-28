--* This Document is AutoGenerate by OrangeFilter, Don't Change it! *

---@meta

---
---[3.2]OrangeFilter Context[parent:]
---
---@class Context
Context = {}

---
---[3.2]get context config
---
--- @return Config#config
--- @nodiscard
function Context:config() end
---
---[3.2]copy texture
---
--- @param arg0 OF_Texture#of_texture
--- @param arg1 OF_Texture#of_texture
--- @return bool
--- @nodiscard
function Context:copyTexture(arg0, arg1) end
---
---[3.2]nv12 to rgb texture
---
--- @param arg0 OF_Texture#of_texture
--- @param arg1 OF_Texture#of_texture
--- @param arg2 OF_Texture#of_texture
--- @return bool
--- @nodiscard
function Context:nv12ToRGB(arg0, arg1, arg2) end
---
---[3.2]mirror texture
---
--- @param arg0 OF_Texture#of_texture
--- @param arg1 OF_Texture#of_texture
--- @param arg2 int
--- @param arg3 int
--- @return bool
--- @nodiscard
function Context:mirrorTexture(arg0, arg1, arg2, arg3) end
---
---[3.2]swap texture rgb
---
--- @param arg0 OF_Texture#of_texture
--- @param arg1 OF_Texture#of_texture
--- @return bool
--- @nodiscard
function Context:swapRGB(arg0, arg1) end
---
---[3.2]blend texture
---
--- @param arg0 OF_Texture#of_texture
--- @param arg1 OF_Texture#of_texture
--- @param arg2 OF_Texture#of_texture
--- @param arg3 OF_BlendMode#of_blendmode
--- @param arg4 float
--- @return bool
--- @nodiscard
function Context:blendTexture(arg0, arg1, arg2, arg3, arg4) end
---
---[3.2]composite texture
---
--- @param arg0 OF_Texture#of_texture
--- @param arg1 OF_Texture#of_texture
--- @param arg2 OF_Texture#of_texture
--- @param arg3 float
--- @param arg4 int
--- @return bool
--- @nodiscard
function Context:compositeTexture(arg0, arg1, arg2, arg3, arg4) end
---
---[3.2]get a texture from pool
---
--- @param arg0 int
--- @param arg1 int
--- @return Texture#texture
--- @nodiscard
function Context:getTexture(arg0, arg1) end
---
---[4.5]get a format texture from pool
---
--- @param arg0 int
--- @param arg1 int
--- @param arg2 int
--- @return Texture#texture
--- @nodiscard
function Context:getTextureFormat(arg0, arg1, arg2) end
---
---[3.2]release texture back to pool
---
--- @param arg0 Texture#texture
--- @nodiscard
function Context:releaseTexture(arg0) end
---
---[3.7]update image data from frameData to opengl texture
---
--- @param arg0 OF_FrameData#frameData
--- @param arg1 OF_Texture#texture
--- @nodiscard
function Context:frameDataToTexture(arg0, arg1) end
---
---[3.2]get program by name
---
--- @param arg0 string
--- @return Program#program
--- @nodiscard
function Context:shaderPass(arg0) end
---
---[3.2]get copy program
---
--- @return Program#program
--- @nodiscard
function Context:sharedCopyPass() end
---
---[3.2]get copy program
---
--- @return Program#program
--- @nodiscard
function Context:sharedCopy2DPass() end
---
---[3.2]get nv12 to rgb program
---
--- @return Program#program
--- @nodiscard
function Context:nv12ToRGBPass() end
---
---[3.2]get swap rgb program
---
--- @return Program#program
--- @nodiscard
function Context:swapRGBPass() end
---
---[3.2]bind frame buffer with texture
---
--- @param arg0 OF_Texture#of_texture
--- @nodiscard
function Context:bindFBO(arg0) end
---
---[3.2]unbind frame buffer
---
--- @nodiscard
function Context:unbindFBO() end
---
---[4.6]draw buffer with muti texture
---
--- @param arg0 int
--- @return int
--- @nodiscard
function Context:drawBufferMRT(arg0) end
---
---[3.2]bind frame buffer with texture and shared depth texture
---
--- @param arg0 OF_Texture#of_texture
--- @nodiscard
function Context:bindFBOWithSharedDepthTexture(arg0) end
---
---[3.6]bind frame buffer with texture and shared depth render buffer
---
--- @param arg0 OF_Texture#of_texture
--- @nodiscard
function Context:bindFBOWithSharedDepthRenderBuffer(arg0) end
---
---[3.2]dettach depth buffer and unbind frame buffer
---
--- @nodiscard
function Context:unbindFBOWithDepthBuffer() end
---
---[3.3]set clear color
---
--- @param arg0 float
--- @param arg1 float
--- @param arg2 float
--- @param arg3 float
--- @nodiscard
function Context:setClearColor(arg0, arg1, arg2, arg3) end
---
---[3.2]clear color buffer
---
--- @nodiscard
function Context:clearColorBuffer() end
---
---[3.2]clear depth buffer
---
--- @nodiscard
function Context:clearDepthBuffer() end
---
---[3.2]clear color and depth buffer
---
--- @nodiscard
function Context:clearColorDepthBuffer() end
---
---[4.7]clear stencil buffer
---
--- @nodiscard
function Context:clearStencilBuffer() end
---
---[4.7]clear color and stencil buffer
---
--- @nodiscard
function Context:clearColorStencilBuffer() end
---
---[3.2]create program by shader source
---
--- @param arg0 string
--- @param arg1 string
--- @return Program#program
--- @nodiscard
function Context:createCustomShaderPass(arg0, arg1) end
---
---[4.6]create program by shader source
---
--- @param arg0 string
--- @param arg1 string
--- @return Program#program
--- @nodiscard
function Context:createCustomShaderPassVersion300(arg0, arg1) end
---
---[3.2]destroy program
---
--- @param arg0 Program#program
--- @nodiscard
function Context:destroyCustomShaderPass(arg0) end
---
---[3.2]get shared quad render
---
--- @return Render#render
--- @nodiscard
function Context:sharedQuadRender() end
---
---[3.2]enable or disable blend
---
--- @param arg0 bool
--- @nodiscard
function Context:setBlend(arg0) end
---
---[3.2]get blend state
---
--- @return bool
--- @nodiscard
function Context:getBlend() end
---
---[3.2]set blend mode
---
--- @param arg0 RS_BlendFunc#rs_blendfunc
--- @param arg1 RS_BlendFunc#rs_blendfunc
--- @nodiscard
function Context:setBlendMode(arg0, arg1) end
---
---[4.4]set blend mode separate
---
--- @param arg0 RS_BlendFunc#rs_blendfunc
--- @param arg1 RS_BlendFunc#rs_blendfunc
--- @param arg2 RS_BlendFunc#rs_blendfunc
--- @param arg3 RS_BlendFunc#rs_blendfunc
--- @nodiscard
function Context:setBlendModeSeparate(arg0, arg1, arg2, arg3) end
---
---[3.4]set cull face mode
---
--- @param arg0 RS_CullMode#rs_cullmode
--- @nodiscard
function Context:setCullFace(arg0) end
---
---[3.4]set depth mask
---
--- @param arg0 bool
--- @nodiscard
function Context:setDepthMask(arg0) end
---
---[3.4]enable depth test or not
---
--- @param arg0 bool
--- @nodiscard
function Context:setDepthTest(arg0) end
---
---[3.4]set depth func
---
--- @param arg0 RS_DepthFunc#rs_depthfunc
--- @nodiscard
function Context:setDepthFunc(arg0) end
---
---[4.7]enable stencil test or not
---
--- @param arg0 bool
--- @nodiscard
function Context:setStencilTest(arg0) end
---
---[4.7]set stencil func
---
--- @param arg0 RS_StencilFunc#rs_stencilfunc
--- @param arg1 int
--- @param arg2 int
--- @nodiscard
function Context:setStencilFunc(arg0, arg1, arg2) end
---
---[4.7]set stencil op
---
--- @param arg0 RS_StencilOp#rs_stencilop
--- @param arg1 RS_StencilOp#rs_stencilop
--- @param arg2 RS_StencilOp#rs_stencilop
--- @nodiscard
function Context:setStencilOp(arg0, arg1, arg2) end
---
---[4.7]set stencil mask
---
--- @param arg0 int
--- @nodiscard
function Context:setStencilMask(arg0) end
---
---[3.2]set viewport
---
--- @param arg0 int
--- @param arg1 int
--- @param arg2 int
--- @param arg3 int
--- @nodiscard
function Context:setViewport(arg0, arg1, arg2, arg3) end
---
---[3.2]set line width
---
--- @param arg0 float
--- @nodiscard
function Context:setLineWidth(arg0) end
---
---[3.2]enable point sprite or not
---
--- @param arg0 bool
--- @nodiscard
function Context:setPointSprite(arg0) end
---
---[3.4]get gl error
---
--- @return int
--- @nodiscard
function Context:getGLError() end
---
---[3.8]get platform
---
--- @return OF_Platform#of_platform
--- @nodiscard
function Context:getPlatform() end
---
---[4.0]get gl version
---
--- @return OF_GL_VERSION#of_gl_version
--- @nodiscard
function Context:getGLVersion() end
---
---[4.0]get glsl Renderer
---
--- @return string
--- @nodiscard
function Context:getRenderer() end
---
---[4.0]get glsl version
---
--- @return OF_GLSL_VERSION#of_glsl_version
--- @nodiscard
function Context:getGLSLVersion() end
---
---[4.6]get max vertex texture image unit count
---
--- @return int
--- @nodiscard
function Context:getMaxVertexTextureImageUnit() end
---
---[4.5]check is support texture half float
---
--- @return bool
--- @nodiscard
function Context:isSupportTextureHalfFloat() end
---
---[4.5]check is support render to half float texture or render buffer
---
--- @return bool
--- @nodiscard
function Context:isSupportColorBufferHalfFloat() end
---
---[4.5]check is support linear filter for half float texture
---
--- @return bool
--- @nodiscard
function Context:isSupportTextureHalfFloatLinear() end
---
---[3.2]create effect from file
---
--- @param arg0 string
--- @param arg1 string
--- @return int
--- @nodiscard
function Context:createEffectFromFile(arg0, arg1) end
---
---[3.2]create effect from json data
---
--- @param arg0 string
--- @param arg1 string
--- @return int
--- @nodiscard
function Context:createEffectFromData(arg0, arg1) end
---
---[4.1]create effect from package
---
--- @param arg0 string
--- @return int
--- @nodiscard
function Context:createEffectFromPackage(arg0) end
---
---[3.2]create empty effect
---
--- @param arg0 string
--- @return int
--- @nodiscard
function Context:createEmptyEffect(arg0) end
---
---[3.2]update effect from file
---
--- @param arg0 int
--- @param arg1 string
--- @param arg2 string
--- @return bool
--- @nodiscard
function Context:updateEffectFromFile(arg0, arg1, arg2) end
---
---[3.2]update effect from json data
---
--- @param arg0 int
--- @param arg1 string
--- @param arg2 string
--- @return bool
--- @nodiscard
function Context:updateEffectFromData(arg0, arg1, arg2) end
---
---[3.2]save effect to file
---
--- @param arg0 int
--- @param arg1 string
--- @return bool
--- @nodiscard
function Context:saveEffectToFile(arg0, arg1) end
---
---[3.2]destroy effect
---
--- @param arg0 int
--- @return bool
--- @nodiscard
function Context:destroyEffect(arg0) end
---
---[3.2]restart effect animation
---
--- @param arg0 int
--- @return OF_Result#of_result
--- @nodiscard
function Context:restartEffectAnimation(arg0) end
---
---[3.2]pause effect animation
---
--- @param arg0 int
--- @return OF_Result#of_result
--- @nodiscard
function Context:pauseEffectAnimation(arg0) end
---
---[3.2]play effect animation
---
--- @param arg0 int
--- @return OF_Result#of_result
--- @nodiscard
function Context:playEffectAnimation(arg0) end
---
---[3.6]set filter float param
---
--- @param arg0 int
--- @param arg1 int
--- @param arg2 string
--- @param arg3 float
--- @nodiscard
function Context:setEffectFilterFloatParam(arg0, arg1, arg2, arg3) end
---
---[3.6]set filter int param
---
--- @param arg0 int
--- @param arg1 int
--- @param arg2 string
--- @param arg3 int
--- @nodiscard
function Context:setEffectFilterIntParam(arg0, arg1, arg2, arg3) end
---
---[3.6]set filter bool param
---
--- @param arg0 int
--- @param arg1 int
--- @param arg2 string
--- @param arg3 int
--- @nodiscard
function Context:setEffectFilterBoolParam(arg0, arg1, arg2, arg3) end
---
---[3.6]set filter enum param
---
--- @param arg0 int
--- @param arg1 int
--- @param arg2 string
--- @param arg3 int
--- @nodiscard
function Context:setEffectFilterEnumParam(arg0, arg1, arg2, arg3) end
---
---[3.6]set filter color param
---
--- @param arg0 int
--- @param arg1 int
--- @param arg2 string
--- @param arg3 float
--- @param arg4 float
--- @param arg5 float
--- @param arg6 float
--- @nodiscard
function Context:setEffectFilterColorParam(arg0, arg1, arg2, arg3, arg4, arg5, arg6) end
---
---[3.6]set filter resource param
---
--- @param arg0 int
--- @param arg1 int
--- @param arg2 string
--- @param arg3 OF_ResType#of_restype
--- @param arg4 string
--- @nodiscard
function Context:setEffectFilterResParam(arg0, arg1, arg2, arg3, arg4) end
---
---[4.5]apply frame
---
--- @param arg0 int
--- @param arg1 OF_FrameData#of_framedata
--- @param arg2 OF_Texture#of_texture)[]
--- @param arg3 OF_Texture#of_texture)[]
--- @return OF_Result#of_result
--- @nodiscard
function Context:applyFrame(arg0, arg1, arg2, arg3) end
---
---[3.2]apply frame with yuv input textures
---
--- @param arg0 int
--- @param arg1 OF_FrameData#of_framedata
--- @param arg2 OF_Texture#of_texture
--- @param arg3 OF_Texture#of_texture
--- @param arg4 OF_Texture#of_texture
--- @param arg5 OF_Texture#of_texture
--- @return OF_Result#of_result
--- @nodiscard
function Context:applyYUV(arg0, arg1, arg2, arg3, arg4, arg5) end
---
---[3.2]apply frame with rgba input texture
---
--- @param arg0 int
--- @param arg1 OF_FrameData#of_framedata
--- @param arg2 OF_Texture#of_texture
--- @param arg3 OF_Texture#of_texture
--- @param arg4 OF_Texture#of_texture
--- @return OF_Result#of_result
--- @nodiscard
function Context:applyRGBA(arg0, arg1, arg2, arg3, arg4) end
---
---[3.2]apply frame with yuv input textures and effects
---
--- @param arg0 int[]
--- @param arg1 int
--- @param arg2 OF_FrameData#of_framedata
--- @param arg3 OF_Texture#of_texture
--- @param arg4 OF_Texture#of_texture
--- @param arg5 OF_Texture#of_texture
--- @param arg6 OF_Texture#of_texture
--- @return OF_Result#of_result
--- @nodiscard
function Context:applyBatchYUV(arg0, arg1, arg2, arg3, arg4, arg5, arg6) end
---
---[3.2]apply frame with rgba input texture and effects
---
--- @param arg0 int[]
--- @param arg1 int
--- @param arg2 OF_FrameData#of_framedata
--- @param arg3 OF_Texture#of_texture
--- @param arg4 OF_Texture#of_texture
--- @param arg5 OF_Texture#of_texture
--- @return OF_Result#of_result
--- @nodiscard
function Context:applyBatchRGBA(arg0, arg1, arg2, arg3, arg4, arg5) end
---
---[3.2]get engine object
---
--- @return Engine#engine
--- @nodiscard
function Context:engine3d() end
---
---[4.0]create texture sheet which rendering image animation
---
--- @return TextureSheet#texturesheet
--- @nodiscard
function Context:createTextureSheet() end
---
---[4.0]destroy texture sheet
---
--- @param arg0 TextureSheet#texturesheet
--- @nodiscard
function Context:destroyTextureSheet(arg0) end
---
---[3.2]create svga from file
---
--- @param arg0 string
--- @param arg1 string
--- @param arg2 bool
--- @return SVGA#svga
--- @nodiscard
function Context:createSVGAFromFile(arg0, arg1, arg2) end
---
---[3.2]destroy svga
---
--- @param arg0 SVGA#svga
--- @return bool
--- @nodiscard
function Context:destroySVGA(arg0) end
---
---[3.2]create texture
---
--- @param arg0 int
--- @param arg1 int
--- @return Texture#texture
--- @nodiscard
function Context:createTexture(arg0, arg1) end
---
---[3.2]create texture
---
--- @param arg0 int
--- @param arg1 int
--- @param arg2 int
--- @param arg3 int
--- @param arg4 int
--- @param arg5 bool
--- @return Texture#texture
--- @nodiscard
function Context:createTextureWithParams(arg0, arg1, arg2, arg3, arg4, arg5) end
---
---[4.5]create texture ref from of_texture
---
--- @param arg0 OF_Texture#of_texture
--- @return Texture#texture
--- @nodiscard
function Context:createTextureRef(arg0) end
---
---[4.5]update texture ref from of_texture
---
--- @param arg0 Texture#texture
--- @param arg1 OF_Texture#of_texture
--- @nodiscard
function Context:updateTextureRef(arg0, arg1) end
---
---[3.7]save texture
---
--- @param arg0 OF_Texture#of_texture
--- @param arg1 string
--- @return bool
--- @nodiscard
function Context:saveTexture(arg0, arg1) end
---
---[4.5]load texture from file: png, jpg(jpeg) or webp
---
--- @param arg0 string
--- @param arg1 int
--- @param arg2 int
--- @param arg3 int
--- @param arg4 bool
--- @param arg5 bool
--- @param arg6 bool
--- @return Texture#texture
--- @nodiscard
function Context:loadTextureFromFilesRGB(arg0, arg1, arg2, arg3, arg4, arg5, arg6) end
---
---[3.2]destroy texture
---
--- @param arg0 Texture#texture
--- @nodiscard
function Context:destroyTexture(arg0) end
---
---[3.2]create LightningRenderer
---
--- @return LightningRenderer#lightningrenderer
--- @nodiscard
function Context:createLightningRenderer() end
---
---[3.2]destroy LightningRenderer
---
--- @param arg0 LightningRenderer#lightningrenderer
--- @nodiscard
function Context:destroyLightningRenderer(arg0) end
---
---[3.2]create RendererTreeNode
---
--- @return RendererTreeNode#renderertreenode
--- @nodiscard
function Context:createRendererTreeNode() end
---
---[3.2]destroy RendererTreeNode
---
--- @param arg0 RendererTreeNode#renderertreenode
--- @nodiscard
function Context:destroyRendererTreeNode(arg0) end
---
---[3.3]create vertex buffer
---
--- @param arg0 FloatArray#floatarray
--- @param arg1 int
--- @return Buffer#buffer
--- @nodiscard
function Context:createVertexBuffer(arg0, arg1) end
---
---[3.3]create index buffer
---
--- @param arg0 Uint16Array#uint16array
--- @param arg1 int
--- @return Buffer#buffer
--- @nodiscard
function Context:createIndexBuffer(arg0, arg1) end
---
---[3.3]destroy buffer
---
--- @param arg0 Buffer#buffer
--- @nodiscard
function Context:destroyBuffer(arg0) end
---
---[3.3]create color render buffer
---
--- @param arg0 int
--- @param arg1 int
--- @return RenderBuffer#renderbuffer
--- @nodiscard
function Context:createColorRenderBuffer(arg0, arg1) end
---
---[3.3]create depth render buffer
---
--- @param arg0 int
--- @param arg1 int
--- @return RenderBuffer#renderbuffer
--- @nodiscard
function Context:createDepthRenderBuffer(arg0, arg1) end
---
---[4.7]create stencil render buffer
---
--- @param arg0 int
--- @param arg1 int
--- @return [RenderBuffer#renderbuffer)
--- @nodiscard
function Context:createStencilRenderBuffer(arg0, arg1) end
---
---[3.3]destroy render buffer
---
--- @param arg0 RenderBuffer#renderbuffer
--- @nodiscard
function Context:destroyRenderBuffer(arg0) end
---
---[3.3]create frame buffer
---
--- @return FrameBuffer#framebuffer
--- @nodiscard
function Context:createFrameBuffer() end
---
---[3.3]destroy frame buffer
---
--- @param arg0 FrameBuffer#framebuffer
--- @nodiscard
function Context:destroyFrameBuffer(arg0) end
---
---[3.3]get shared frame buffer
---
--- @return FrameBuffer#framebuffer
--- @nodiscard
function Context:sharedFrameBuffer() end
---
---[4.1]get shared frame buffer id
---
--- @return int
--- @nodiscard
function Context:sharedFrameBufferID() end
---
---[4.3]create game from file
---
--- @param arg0 string
--- @param arg1 string
--- @return int
--- @nodiscard
function Context:createGameFromFile(arg0, arg1) end
---
---[4.3]destroy game
---
--- @param arg0 int
--- @return bool
--- @nodiscard
function Context:destroyGame(arg0) end
---
---[4.3]start game
---
--- @param arg0 int
--- @return OF_Result#of_result
--- @nodiscard
function Context:startGame(arg0) end
---
---[4.3]pause game
---
--- @param arg0 int
--- @return OF_Result#of_result
--- @nodiscard
function Context:pauseGame(arg0) end
---
---[4.3]resume game
---
--- @param arg0 int
--- @return OF_Result#of_result
--- @nodiscard
function Context:resumeGame(arg0) end
---
---[4.3]stop game
---
--- @param arg0 int
--- @return OF_Result#of_result
--- @nodiscard
function Context:stopGame(arg0) end
---
---[4.3]send message
---
--- @param arg0 int
--- @param arg1 string
--- @return OF_Result#of_result
--- @nodiscard
function Context:sendMessage(arg0, arg1) end
---
---[4.4]load text from file
---
--- @param arg0 string
--- @return string
--- @nodiscard
function Context:loadTextFromFile(arg0) end
---
---[4.5]get current time
---
--- @param arg0 double
--- @nodiscard
function Context:currentTime(arg0) end
---
---[4.7]get font atlas cache
---
--- @return FontAtlasCache#fontatlascache
--- @nodiscard
function Context:getFontAtlasCache() end
---
---[4.10]get opengl checker
---
--- @return GlChecker#glchecker
--- @nodiscard
function Context:glChecker() end

return Context
