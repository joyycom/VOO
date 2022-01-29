/***************** BEGIN FILE HRADER BLOCK *********************************
*
* \author Cheng Yu <chengyu@yy.com> phone:13710201865
*
* Copyright (C) 2016-2020 JOYY.Inc
* All rights reserved.
*
* This library is free software; you can redistribute it and/or modify it
* under the terms of the GNU Lesser General Public License as published by
* the Free Software Foundation; either version 3 of the License, or (at
* your option) any later version. Please review the following information
* to ensure the GNU Lesser General Public License version 3 requirements
* will be met: https://www.gnu.org/licenses/lgpl.html.
*
* You should have received a copy of the GNU Lesser General Public License
* along with this library; if not, write to the Free Software Foundation.
*
* If you use the source in a your project, please consider send an e-mail
* to me, and allow me to add your project to my home page.
*
***************** END FILE HRADER BLOCK ***********************************/

#pragma once
#include "apiprivate.h"
#include "gldefine.h"
#include <memory>
#include <vector>

namespace OrangeFilter
{

class Context;
class TexturePrivate;
class ImageLoadData;
class OF_EFFECT_API Texture: public ITexture, public ResObj
{
    OF_DECLARE_PRIVATE(Texture)
public:
    Texture(OFUInt32 target);
    Texture(Context* context, OFUInt32 target);
    Texture(Context* context, int width, int height, int format, OFUInt32 target, OFUInt32 textureId);

    OFUInt32 textureID() const;
    OFUInt32 target() const;
    OFInt32 format() const;
    int width() const;
    int height() const;

    void createFromRef(int width, int height, int format, int target, OFUInt32 textureId);
    void create(int width, int height, int format, const void* data = OF_NULL, int filterType = GL_LINEAR, int wrapType = GL_CLAMP_TO_EDGE, bool isMipmap = false);
    void create(ImageLoadData* pData, bool bAlpha, int filterType = GL_LINEAR, int wrapType = GL_CLAMP_TO_EDGE, bool isMipmap = false);
#if defined(OF_USE_GLES3) || defined(OF_USE_GLES31)
    void create3D(int width, int height, int depth, int internalFormat, int format, int type, const void* data, int filterType = GL_LINEAR, int wrapType = GL_CLAMP_TO_EDGE, bool isMipmap = false);
#endif
    void updateRef(int width, int height, int format, int target, OFUInt32 textureId);
    void update(int xoffset, int yoffset, int width, int height, const void* data);
    void update(int xoffset, int yoffset, int width, int height, int format, int type, const void* data);/*Deprecated*/
    void updateCubeFace(OFUInt32 face, int level, int width, int height, const void* data);

    void* getData();
    void setData(void* data, int size);
    void getDataCopy(OFUInt8* buffer, int bufferOffset, int dataOffset, int size);
    void setSampler(OFInt32 filterMode, OFInt32 wrapMode);

    // multi-frame textures
    void setFrameTimes(const std::vector<int>& frameTimes);
    const std::vector<int>& getFrameTimes();
    void updateFrame(int frameIndex);

    void genMipmap();

    void updateCompressed(int width, int height, int size, const void* data);
    void updateCubeFaceCompressed(OFUInt32 face, int level, int width, int height, int size, const void* data);

    void tearDown();
    OFUInt32 bindFBO(OFUInt32 fbo);

    OF_Texture toOFTexture();
    void toData(OF_Texture* tex) const;
    bool saveToPng(const char* path, OFUInt32 fbo);
    bool saveToJpeg(const char* path, OFUInt32 fbo, int quality=75/*quality is between 1 and 100.*/);
    bool copyTexToMemry(unsigned char* data, OFUInt32 fbo, OF_PixelFormat format);

    bool isCompressed();

	void decRefCount();
	int incRefCount() const;
protected:
    virtual ~Texture();
};
}
