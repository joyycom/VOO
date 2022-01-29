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
#ifndef _OF_POSE_H_
#define _OF_POSE_H_

#include "of_core.h"

#ifdef __cplusplus
extern "C" {
#endif

#ifndef OF_MODULE_NO_POSE
OF_POSE_EXPORT_API OF_Result OF_HeadPoseEstimate(
    OFHandle contextID,                         /* [in] */
    OF_FrameData* frameData);                   /* [out] */

OF_POSE_EXPORT_API OF_Result OF_ResetHeadPoseEstimate(void);

OF_POSE_EXPORT_API OF_Result OF_SetCheckHeadPoseEstimateResult(
    OFHandle contextID,                         /* [in] */
    OFBool isisCheckResultOn);                  /* [in] */

/*
   Deprecated, use OF_PrepareFrameData replace. 
 */
OF_POSE_EXPORT_API OF_Result OF_PrepareEffectFrameData(
    OFHandle contextID,                         /* [in] */
    OFHandle effectID,                          /* [in] */
    OF_FrameData* frameData);                   /* [out] */

/* Compatibility with older api. */
#define OF_PrepareFrameData    OF_HeadPoseEstimate

#endif //OF_MODULE_NO_POSE

#ifdef __cplusplus
}
#endif

#endif // _OF_POSE_H_