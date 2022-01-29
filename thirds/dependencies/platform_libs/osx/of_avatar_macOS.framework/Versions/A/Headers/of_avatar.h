/***************** BEGIN FILE HRADER BLOCK *********************************
 *
 * \author Cheng Yu <chengyu@yy.com> phone:13710201865
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
#ifndef _OF_AVATAR_H_
#define _OF_AVATAR_H_

#include "of_core.h"

#ifdef __cplusplus
extern "C" {
#endif

#ifndef OF_MODULE_NO_AVATAR
typedef enum _OF_AvatarMode
{
    OF_AvatarMode_FaceBlendshape = 0x00000001,
    OF_AvatarMode_FaceMesh       = 0x00000002,
    OF_AvatarMode_FaceLandmark   = 0x00000004,
    OF_AvatarMode_All            = 0xFFFFFFFF
} OF_AvatarMode;

typedef enum _OF_AvatarConfig
{
    OF_AvatarConfig_IsGenderMale = 0,  // true-male, false-female, used in reconstruct.
    OF_AvatarConfig_IsTrackEyes,       // track eyes or not, used in evaluate expression.
    OF_AvatarConfig_IsSkipFrame        // skip frame for better performance
} OF_AvatarConfig;

/************************************************************************/
/* Some base interface definition.                                      */
/************************************************************************/
OF_AVATAR_EXPORT_API OF_Result OF_CreateAvatar(
    const char* modelPath,
    int mode, // AvatarMode
    OFHandle* avaterId);

OF_AVATAR_EXPORT_API OF_Result OF_DestroyAvatar(
    OFHandle avaterId);

OF_AVATAR_EXPORT_API OF_Result OF_ApplyAvatar(
    OFHandle avatarId, 
    OF_FrameData* data);

OF_AVATAR_EXPORT_API OF_Result OF_SetAvatarConfig(
    OFHandle avatarId,
    OF_AvatarConfig cfg,
    void* value
);

OF_AVATAR_EXPORT_API void OF_ResetAvatar(
    OFHandle avatarId);

OF_AVATAR_EXPORT_API OF_Result OF_ReconstructAvatar(
    const OFHandle avatarId,                  // [in]
    OF_FrameData* data,						  // [in]
    const char* avatarOutFile                 // [in]
);

OF_AVATAR_EXPORT_API OF_Result OF_FaceDetect(
    OFHandle avatarId,
    OF_FrameData* data);

/************************************************************************/
/* Internal use.                                                        */
/************************************************************************/
#define KFilterAvatar   "AvatarFilter"
#define KFilterAvatarFaceMesh "AvatarFaceMeshFilter"
#define WEIGHT_NUM  51

/* KFilterAvatar */
typedef struct _OF_AvatarFilterExtraData
{
    OFPath meshPath;
    OFPath texturePath;
    OFUInt8 expression;
    float weight;
    float enlarge;
    bool wireframe;
    bool automatic;
    bool mirror;
} OF_AvatarFilterExtraData;

/* KFilterAvatarFaceMesh */
typedef struct _OF_AvatarFaceMeshFilterExtraData
{
    OFPath meshPath;
    OFUInt32 pointIndex;
} OF_AvatarFaceMeshFilterExtraData;

typedef enum _OF_ParamFilterAvatarFaceMesh
{
    OF_ParamFilterFaceMesh_TranslateX = 0,
    OF_ParamFilterFaceMesh_TranslateY,
    OF_ParamFilterFaceMesh_TranslateZ,
    OF_ParamFilterFaceMesh_RotateX,
    OF_ParamFilterFaceMesh_RotateY,
    OF_ParamFilterFaceMesh_RotateZ,
    OF_ParamFilterFaceMesh_ScaleX,
    OF_ParamFilterFaceMesh_ScaleY,
    OF_ParamFilterFaceMesh_ScaleZ,
    OF_ParamFilterFaceMesh_Count
} OF_ParamFilterAvatarFaceMesh;

#endif //OF_MODULE_NO_AVATAR

#ifdef __cplusplus
}
#endif

#endif // _OF_AVATAR_H_