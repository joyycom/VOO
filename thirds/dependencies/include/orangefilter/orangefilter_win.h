/***************** BEGIN FILE HRADER BLOCK *********************************
 *
 *
 * Copyright (C) 2016-2020 JOYY Inc.
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
#ifndef _ORANGEFILTER_WIN_
#define _ORANGEFILTER_WIN_

#include "orangefilter.h"

/************************************************************************/
/* Module export definition.                                            */
/************************************************************************/
#ifdef __cplusplus
extern "C" {
#endif
	//
	//These API is for input data is image data not texture
	//

	//
	//Init 
	//
	OF_EFFECT_API OF_Result OFW_Init(OFHandle *contextid);

	//
	//Uninit
	OF_EFFECT_API OF_Result OFW_Uninit(OFHandle contextid);

	//
	//@brief use image data as input to effect process
	//@param[in] effectID is an ID created by OF_CreateFilter for effect
	//@param[in] srcData is source image data
	//@param[in] dstData is output data which is processed by effect, dstData's format is OF_PixelFormat_RGB32
	//@param[in] width is image width
	//@param[in] height is image height
	//@param[in] format is image format, support OF_PixelFormat_BGR32 and OF_PixelFormat_RGB32 by this func
	//@param[in] faceData is face alignment data
	OF_EFFECT_API OF_Result OFW_ApplyEffectRGB(
		OFHandle contextid,							/* [in] */
		OFHandle effectID,                          /* [in] */
		OFUInt8* srcData,							/* [in] */
		OFUInt8* dstData,							/* [in] */
		OFInt32 width,								/* [in] */
		OFInt32 height,								/* [in] */
		OF_PixelFormat format,						/* [in] */ 
		OF_FrameData* frameData);					/* [in] */

    OF_EFFECT_API OF_Result OFW_Applying(
        OFHandle contextid,							/* [in] */
        OFHandle* effectIDList,                     /* [in] */
        OF_Result * effectIDResult,                 /* [in] */
        OFUInt16 effectIDCount,                     /* [in] */
        OFUInt8* srcData,							/* [in] */
        OFUInt8* dstData,							/* [in] */
        OFInt32 width,								/* [in] */
        OFInt32 height,								/* [in] */
        OF_PixelFormat format,						/* [in] */
        OF_FrameData* frameData,					/* [in] */
        OFHandle gameID = OF_INVALID_HANDLE);       /* [in] */

    OF_EFFECT_API OF_Result OFW_OpenGLVersion(char* str, int length);
    OF_EFFECT_API OF_Result OFW_SetPBOUpdateFlag(OFHandle contextid);
    OF_EFFECT_API OF_Result OFW_SetPBOSupportedFlag(OFHandle contextid, OFBool flag);
#ifdef __cplusplus
}
#endif

#endif