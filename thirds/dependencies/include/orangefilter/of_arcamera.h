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
#ifndef _OF_ARCAMERA_H_
#define _OF_ARCAMERA_H_

#include "of_core.h"

#ifdef __cplusplus
extern "C" {
#endif

/************************************************************************/
/* Global enum definition.                                              */
/************************************************************************/
typedef enum _OF_ArCameraType
{
    OF_ArCameraType_ARKit = 0,  // for ios
    OF_ArCameraType_ARCore = 1, // for android
	OF_ArCameraType_GYRO = 2,
    OF_ArCameraType_IMU = 3,
    OF_ArCameraType_SLAM = 4,
    OF_ArCameraType_Marker = 5,
    OF_ArCameraType_MarkerLess = 6,
} OF_ArCameraType;
/************************************************************************/
/* Some base interface definition.                                      */
/************************************************************************/
typedef struct _OF_ArCameraParams
{
    OF_ArCameraType type;
    const char* (*callback)(OFHandle context, OFHandle arCameraID, const char* json);
} OF_ArCameraParams;

/************************************************************************/
/* Some base interface definition.                                      */
/************************************************************************/
OF_ARCAMERA_EXPORT_API OF_Result OF_CreateArCamera(
	OFHandle contextID,
	OF_ArCameraParams param,                   /* [in] */
	OFHandle* arCameraID);					   /* [out] */

OF_ARCAMERA_EXPORT_API OF_Result OF_DestroyArCamera(
	OFHandle contextID,							/* [in] */
	OFHandle arCameraID);                       /* [in] */

/* obj is different according to the platform and ArCameraType
 OF_ArCameraType_ARKIt, obj type is ARFrame
 */
OF_ARCAMERA_EXPORT_API OF_Result OF_ApplyArCamera(
	OFHandle contextID,							/* [in] */
	OFHandle arCameraID,                        /* [in] */
    void *obj,                                  /* [in] */
	OF_ArCameraData* outData);                  /* [in/out] */
    
OF_ARCAMERA_EXPORT_API void OF_FillInvalidArCameraData(
	OF_ArCameraData* outData);					/* [in/out] */

#ifdef __cplusplus
}

#endif

#endif // _OF_ARCAMERA_H_
