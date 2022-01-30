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
#ifndef _OF_CORE_H_
#define _OF_CORE_H_

/************************************************************************/
/* Module export definition.                                            */
/************************************************************************/
#if defined(OF_WIN32) || defined(_WIN32) || defined(WIN32)
#   ifdef OF_CORE_LIB
#       define OF_CORE_API __declspec(dllexport)
#   else
#       define OF_CORE_API __declspec(dllimport)
#   endif
#else
#   define OF_CORE_API
#endif

#if defined(OF_WIN32) || defined(_WIN32) || defined(WIN32)
#   ifdef OF_CORE_LIB
#       define OF_CORE_EXPORT_API __declspec(dllexport)
#   else
#       define OF_CORE_EXPORT_API __declspec(dllimport)
#   endif
#elif defined(OF_ANDROID)
#   define OF_CORE_EXPORT_API __attribute__((visibility("default")))
#else
#   define OF_CORE_EXPORT_API
#endif


#if defined(OF_WIN32) || defined(_WIN32) || defined(WIN32)
#   ifdef OF_EFFECT_LIB
#       define OF_EFFECT_API __declspec(dllexport)
#   else
#       define OF_EFFECT_API __declspec(dllimport)
#   endif
#else
#   define OF_EFFECT_API
#endif

#if defined(OF_WIN32) || defined(_WIN32) || defined(WIN32)
#   ifdef OF_EFFECT_LIB
#       define OF_EFFECT_EXPORT_API __declspec(dllexport)
#   else
#       define OF_EFFECT_EXPORT_API __declspec(dllimport)
#   endif
#elif defined(OF_ANDROID)
#   define OF_EFFECT_EXPORT_API __attribute__((visibility("default")))
#else
#   define OF_EFFECT_EXPORT_API
#endif

#if defined(OF_WIN32) || defined(_WIN32) || defined(WIN32)
#   ifdef OF_POSE_LIB
#       define OF_POSE_EXPORT_API __declspec(dllexport)
#   else
#       define OF_POSE_EXPORT_API __declspec(dllimport)
#   endif
#elif defined(OF_ANDROID)
#   define OF_POSE_EXPORT_API __attribute__((visibility("default")))
#else
#   define OF_POSE_EXPORT_API
#endif

#if defined(OF_WIN32) || defined(_WIN32) || defined(WIN32)
#   ifdef OF_ARCAMERA_LIB
#       define OF_ARCAMERA_EXPORT_API __declspec(dllexport)
#   else
#       define OF_ARCAMERA_EXPORT_API __declspec(dllimport)
#   endif
#elif defined(OF_ANDROID)
#   define OF_ARCAMERA_EXPORT_API __attribute__((visibility("default")))
#else
#   define OF_ARCAMERA_EXPORT_API
#endif

#if defined(OF_WIN32) || defined(_WIN32) || defined(WIN32)
#   ifdef OF_AVATAR_LIB
#       define OF_AVATAR_EXPORT_API __declspec(dllexport)
#   else
#       define OF_AVATAR_EXPORT_API __declspec(dllimport)
#   endif
#elif defined(OF_ANDROID)
#   define OF_AVATAR_EXPORT_API __attribute__((visibility("default")))
#else
#   define OF_AVATAR_EXPORT_API
#endif

#ifdef __cplusplus
extern "C" {
#endif
/************************************************************************/
/* Basic types                                                          */
/************************************************************************/
#if defined(OF_WIN32) || defined(_WIN32) || defined(WIN32)
typedef __int8                              OFInt8;
typedef __int16                             OFInt16;
typedef __int32                             OFInt32;
typedef __int64                             OFInt64;
typedef unsigned __int8                     OFUInt8;
typedef unsigned __int16                    OFUInt16;
typedef unsigned __int32                    OFUInt32;
typedef unsigned __int64                    OFUInt64;
#ifdef _WIN64
typedef unsigned __int64                    OFUIntPtr;
typedef __int64                             OFIntPtr;
#else  /* _WIN64 */
typedef __w64 unsigned int                  OFUIntPtr;
typedef __w64 int                           OFIntPtr;
#endif  /* _WIN64 */
#else
#include <stdint.h>
typedef int8_t                              OFInt8;
typedef int16_t                             OFInt16;
typedef int32_t                             OFInt32;
typedef int64_t                             OFInt64;
typedef uint8_t                             OFUInt8;
typedef uint16_t                            OFUInt16;
typedef uint32_t                            OFUInt32;
typedef uint64_t                            OFUInt64;
typedef uintptr_t                           OFUIntPtr;
typedef intptr_t                            OFIntPtr;
#endif
typedef int                                 OFBool;
typedef char                                OFPath[ 1024 ];
typedef char                                OFText[ 1024 ];
typedef char                                OFName[ 256 ];
typedef char                                OFUUID[ 64 ];
typedef OFUInt32                            OFHandle;

/************************************************************************/
/* Macros for utilites.                                                 */
/************************************************************************/
#define OF_VERSION                          "4.10.7"
#define OF_EFFECT_VERSION                   "4.10"
#define OF_NULL                             0
#define OF_TRUE                             1
#define OF_FALSE                            0
#define OF_INVALID_HANDLE                   0

/************************************************************************/
/* Global enum definition.                                              */
/************************************************************************/
typedef enum _OF_Result
{
    OF_Result_Success = 0,
    OF_Result_Failed,
    OF_Result_NotInit,
    OF_Result_InvalidInput,
    OF_Result_InvalidFrameHandler,
    OF_Result_InvalidFilter,
    OF_Result_InvalidEffect,
    OF_Result_AnimationStoped,
    OF_Result_OpenEffectFileError,
    OF_Result_InvalidEffectVersion,
    OF_Result_InvalidFilterType,
    OF_Result_InvalidSceneType,
    OF_Result_ParseDataError,
    OF_Result_ResNotExist
} OF_Result;

typedef enum _OF_LogLevel
{
    OF_LogLevel_Info    = 0x00000001,
    OF_LogLevel_Warn    = 0x00000002,
    OF_LogLevel_Error   = 0x00000004,
    OF_LogLevel_Debug   = 0x00000008,
    OF_LogLevel_Trace   = 0x00000010,
    OF_LogLevel_Verbose = 0xFFFFFFFF
} OF_LogLevel;

typedef enum _OF_GestureType
{
    OF_GestureType_Unknow = 0,
    OF_GestureType_V,
    OF_GestureType_ThumbUp,
    OF_GestureType_OneHandHeart,
    OF_GestureType_Lift,
    OF_GestureType_TwoHandHeart,
    OF_GestureType_Size,

    OF_GestureType_HappyNewYear_start = 20,
    OF_GestureType_Palm,
    OF_GestureType_ZuoYi,
    OF_GestureType_HappyNewYear_end,

    OF_GestureType_Palm_start = 30,
    OF_GestureType_PalmClose,
    OF_GestureType_PalmOpen,
    OF_GestureType_PalmTogether,
    OF_GestureType_Palm_end,

    OF_GestureType_Retina_start = 35,
    OF_GestureType_Retina_Unknow,           // ��Ŀ������
    OF_GestureType_Retina_V,                // ��V
    OF_GestureType_Retina_ThumbUp,          // ����
    OF_GestureType_Retina_OneHandHeart,     // ���ֱ���
    OF_GestureType_Retina_Spiderman,        // ֩����
    OF_GestureType_Retina_Lift,             // �о�
    OF_GestureType_Retina_666,              // 666
    OF_GestureType_Retina_TwoHandHeart,     // ˫�ֱ���
    OF_GestureType_Retina_ZuoYi,            // ��ȭ
    OF_GestureType_Retina_PalmOpen,         // ����
    OF_GestureType_Retina_PalmTogether,     // ��ʮ
    OF_GestureType_Retina_OK,               // OK
    OF_GestureType_Retina_Pistol,           // ��ָǹ   // 2019.04.24��right now venus don't have this type and the following, it will support in the future
    OF_GestureType_Retina_FingerIndex,      // ʳָָ��
    OF_GestureType_Retina_Fist,             // ȭͷ����
    OF_GestureType_Retina_ILoveYou,         // I Love You
    OF_GestureType_Retina_end,
} OF_GestureType;
    
typedef enum _OF_LimbsType 
{
    OF_GestureType_Hand    = 0,
    OF_GestureType_Head    = 1,
    OF_GestureType_Foot    = 2,
    OF_GestureType_UnKnown = 10,
} OF_LimbsType;

typedef enum _OF_PostureType
{
    OF_PostureType_A        = 0,
    OF_PostureType_B        = 1,
    OF_PostureType_C        = 2,
    OF_PostureType_MaxCount = 3,
    OF_PostureType_UnKnown = 10,
} OF_PostureType;

typedef enum _OF_ROTATE_TYPE
{
    OF_ROTATE_TYPE_ROTATE_0   = 0,        /* The image does not need to be rotated, and the face in the image is a positive face. */
    OF_ROTATE_TYPE_ROTATE_90  = 1,        /* The image needs to be rotated 90 degrees clockwise to make the face in the image positive. */
    OF_ROTATE_TYPE_ROTATE_180 = 2,        /* The image needs to be rotated 180 degrees clockwise to make the face in the image positive. */
    OF_ROTATE_TYPE_ROTATE_270 = 3         /* The image needs to be rotated 270 degrees clockwise to make the face in the image positive. */
} OF_Rotate_Type;

/************************************************************************/
/* Some base interface definition.                                      */
/************************************************************************/

typedef struct _OF_Rect
{
    double x;
    double y;
    double width;
    double height;
    float angle;
} OF_Rect;

typedef struct _OF_Texture
{
    OFUInt32 textureID;                     /* OpenGL texture id.*/
    OFUInt32 textureID_a;                   /* OpenGL texture id.*/
    OFUInt32 target;                        /* OpenGL texture target, e.g. GL_TEXTURE_2D, GL_TEXTURE_EXTERNAL_OES.*/
    OFUInt32 format;                        /* OpenGL texture format, e.g. GL_RGBA.*/
    OFUInt32 pixelComponentFormat;          /* OpenGL texture pixel component size. e.g. rgba4444 texture format is GL_RGBA, pixelComponentFormat is _GL_UNSIGNED_SHORT_4_4_4_4 */
    OFInt32 width;                          /* OpenGL texture width.*/
    OFInt32 height;                         /* OpenGL texture height.*/
} OF_Texture;

typedef struct _OF_HeadPose
{
    float modelViewMat[16];
    float projectionMat[16];
} OF_HeadPose;

typedef struct _OF_FaceMesh
{
    float* vertices;                        /* Deprecated */
    OFUInt32 verticesCount;                 /* Deprecated */
    OFUInt8* textureData;                   /* Deprecated */
    OFUInt8* textureDataExt;                /* Deprecated */
    OFInt32 textureWidth;                   /* Deprecated */
    OFInt32 textureHeight;                  /* Deprecated */
    float modelViewMat[16];
    float projectionMat[16];
} OF_FaceMesh;

typedef struct _OF_FaceFrameExtData
{
    float facePoints[106 * 2];
    OFUInt32 facePointsCount;
    float face3dPoints[106 * 3];
    OFUInt32 face3dPointsCount;
    float blendshapeCoeffs[51];
} OF_FaceFrameExtData;

typedef struct _OF_FaceFrameData
{
    float box[4];                           /* [left, top, width, height]. */
    float facePoints[106 * 2];              /* [f0x, f0y, f1x, f1y, f2x, f2y, ...]. */
    float extraFacePoints[134 * 2];         /* extra face data */
    float eyeballPoints[40 * 2];            /* eyeball data */
    float score;
    OFUInt32 gender;
    OFUInt32 beautyScore;
    OFUInt32 facePointsCount;
    OFUInt32 extraFacePointsCount;
    OFUInt32 eyeballPointsCount;
    OFBool isMouthOpen;
    OFBool isEyeBlink;
    OFBool isHeadYaw;
    OFBool isHeadPitch;
    OFBool isBrowJump;
    float openMouthIntensity;
    OF_HeadPose headPose;
    OF_FaceMesh faceMesh;
    float blendshapeWeightMap[51];
    float blendshapeWeightTongue;
    float leftEyeRotation[3];               // left eye local rotation in radians
    float rightEyeRotation[3];              // right eye local rotation in radians
    float headRotation[3];                  // head rotation
	OF_Texture maskTexture;
	float maskRect[4];
} OF_FaceFrameData;

#define OF_MAX_FACEFRAMEDATA_SIZE  5
typedef struct _OF_FaceFrameDataArr
{
    OF_FaceFrameData faceItemArr[OF_MAX_FACEFRAMEDATA_SIZE];
    OFUInt32 faceCount;
    OFInt32 faceUse;
} OF_FaceFrameDataArr;

typedef struct _OF_GestureFrameData
{
    OF_GestureType type;                    /* Gesture type */
    float x;                                /* X coordinate of gesture box' center */
    float y;                                /* Y coordinate of gesture box' center */
    float width;                            /* Width of gesture box' center */
    float height;                           /* Height of gesture box' center */
    float score;
} OF_GestureFrameData;

#define OF_MAX_GESTUREFRAMEDATA_SIZE  10
typedef struct _OF_GestureFrameDataArr
{
    OFUInt32 gestureCount;                                         /* Gesture count */
    OF_GestureFrameData gestureArr[OF_MAX_GESTUREFRAMEDATA_SIZE];  /* Gesture data array */
} OF_GestureFrameDataArr;

#define OF_MAX_BODYLANDMARKS_NUMB     32
#define OF_MAX_BODYCONTOUR_NUMB       65
typedef struct _OF_BodyFrameData
{
    float bodyPoints[OF_MAX_BODYLANDMARKS_NUMB * 2];               /* [f0x, f0y, f1x, f1y, f2x, f2y, ...]. */
    float bodyPointsScore[OF_MAX_BODYLANDMARKS_NUMB];
    OFUInt32 bodyPointsCount;
    float contourPoints[OF_MAX_BODYCONTOUR_NUMB * 2];
    float contourPointsScore[OF_MAX_BODYCONTOUR_NUMB];
    OFUInt32 contourPointsCount;
    OF_PostureType postureType;
} OF_BodyFrameData;

typedef struct _OF_Body3DFrameData
{
    float body3DPoints[OF_MAX_BODYLANDMARKS_NUMB * 3];
    float body3DPointsOrientation[OF_MAX_BODYLANDMARKS_NUMB * 4];
    float body3DPointsScore[OF_MAX_BODYLANDMARKS_NUMB];
    OFUInt32 body3DPointsCount;
    OF_PostureType postureType;
} OF_Body3DFrameData;

#define OF_MAX_BODYFRAMEDATA_SIZE  5
typedef struct _OF_BodyFrameDataArr
{
    OF_BodyFrameData bodyItemArr[OF_MAX_BODYFRAMEDATA_SIZE];
    OFUInt32 bodyCount;
} OF_BodyFrameDataArr;

typedef struct _OF_Body3DFrameDataArr
{
    OF_Body3DFrameData body3DItemArr[OF_MAX_BODYFRAMEDATA_SIZE];
    OFUInt32 bodyCount;
} OF_Body3DFrameDataArr;

typedef struct _OF_SegmentFrameData
{
    OF_Texture segmentTexture;              /* [in] [out]. Weight map texture of body segment result. */
    OF_Texture hairTexture;                 /* [in] [out]. Weight map texture of hair segment result. */
    OF_Texture clothesTexture;              /* [in] [out]. Weight map texture of clothes segment result. */
} OF_SegmentFrameData;

typedef struct _OF_AudioFrameData
{
    float beat;
    float loudness;
    float loudnessSmooth;
    float microphoneVolume;
    OFUInt8 waveformData[512];
    OFUInt8 frequencyData[512];
    OFUInt32 frequencyDataCount;
} OF_AudioFrameData;

typedef enum _OF_ArPlaneAnchorAlignment
{
    OF_ArPlaneAnchorAlignment_Horizontal = 0,
    OF_ArPlaneAnchorAlignment_Vertical   = 1,
} OF_ArPlaneAnchorAlignment;
    
typedef enum _OF_ArCameraDataState
{
    OF_ArCameraDataState_Invalid = 0,
    OF_ArCameraDataState_Normal  = 1,
} OF_ArCameraDataState;

typedef struct _OF_ArPlaneAnchor
{
    OF_ArPlaneAnchorAlignment alignment;
    char identifier[64];
    float transform[16];
    float center[3];
    float extent[3];
} OF_ArPlaneAnchor;

#define OF_MAX_ARPLANEANCHOR_SIZE  5
typedef struct _OF_ArCameraData
{
    OF_ArCameraDataState state;
    float transform[16];
    float eulerAngles[3];
    OFUInt32 planeAnchorCount;
    OF_ArPlaneAnchor planeAnchors[OF_MAX_ARPLANEANCHOR_SIZE];
    
    /* for Lua binding  */
    struct {
        OFHandle arCameraID;
        OFHandle contextID;
        void *obj;
    } luaBindingData;
} OF_ArCameraData;

typedef struct _OF_Variant OF_Variant;
typedef struct _OF_FrameData
{
    OFUInt8* imageData;                     /* [in] Image raw buffer, some filter may need this, most of the filter is set to OF_NULL. */
    OFInt32 width;                          /* [in] Width of data. */
    OFInt32 height;                         /* [in] Height of data. */
    OFInt32 widthStep;                      /* [in] The number of bytes in per line of the data. */
    OFInt32 format;                         /* [in] Format of input image data, One of the OF_PixelFormat. */
    OF_Rotate_Type rotateType;              /* [in] The rotate type of input image data, One of the OF_Rotate_Type. */
    float timestamp;                        /* [in] The timestamp of Frame Data, unit is seconds. */
    OF_FaceFrameDataArr faceFrameDataArr;   /* [in] [out] Face alignment data. */
    OF_GestureFrameDataArr gestureDataArr;  /* [in] [out] Gesture data. */
    OF_BodyFrameDataArr bodyFrameDataArr;   /* [in] [out] Body2D data. */
    OF_Body3DFrameDataArr body3DFrameDataArr;/*[in] [out] Body3D data.*/
    OF_SegmentFrameData segmentFrameData;   /* [in] [out] Segment data. */
    OF_AudioFrameData audioFrameData;       /* [in] [out] Audio data, e.g from music rhythm. */
    OF_ArCameraData arCameraData;           /* [in] [out] Ar Camera */
    float cameraMat[16];                    /* [in] Some AR filter */
    OFBool trackOn;                         /* [in] Has track camera matrix data in cameraMat. */
    OFInt32 curNode;                        /* [out] */
    float pickPoint[2];                     /* [in] [out] */
    OFBool pickOn;                          /* [in] */
    OFBool pickResult;                      /* [out] */
    OF_Texture weightMapTexture;            /* [in] Deprecated. */
    OFInt32 uMattingBlendMode;              /* [in] Deprecated. */
    OF_Variant *extData;
    OFBool extHasConvert;                      /* [in] extData has converted. */
} OF_FrameData;

/************************************************************************/
/* Global exported API definition.                                      */
/************************************************************************/
typedef void(*LogCallback) (const char* fmtMsg);
typedef void(*LogCallback2) (const char* fmtMsg, const OFInt32 currentLogLevel);

OF_CORE_EXPORT_API OF_Result OF_SetLogCallback(
    LogCallback logFun);                    /* [in] */

OF_CORE_EXPORT_API OF_Result OF_SetLogCallback2(
    LogCallback2 logFun);                   /* [in] */

OF_CORE_EXPORT_API OF_Result OF_SetLogLevel(
    OFUInt32 logLevel);                     /* [in] */

OF_CORE_EXPORT_API OF_Result OF_GetLogLevel(
    OFUInt32* logLevel);                    /* [out] Default = OF_LogLevel_Error | OF_LogLevel_Warn | OF_LogLevel_Info. */

typedef struct _OF_StringHolder OF_StringHolder;
OF_CORE_EXPORT_API OF_StringHolder *OF_StringHolder_Create(const char *str, int size);
OF_CORE_EXPORT_API void OF_StringHolder_Retain(OF_StringHolder *ref);
OF_CORE_EXPORT_API void OF_StringHolder_Release(OF_StringHolder *ref);
OF_CORE_EXPORT_API const char *OF_StringHolder_GetString(OF_StringHolder *ref);
OF_CORE_EXPORT_API OFInt32 OF_StringHolder_GetSize(OF_StringHolder *ref);

typedef enum {
    OF_VariantType_Null,
    OF_VariantType_Bool,
    OF_VariantType_Int64,
    OF_VariantType_Double,
    OF_VariantType_String,
    OF_VariantType_Array,
    OF_VariantType_Object,
    OF_VariantType_Binary,
} OF_VariantType;

typedef struct _OF_Binary {
    OFUInt8 *data;
    int size;
} OF_Binary;

OF_CORE_EXPORT_API OF_Variant *OF_Variant_CreateDouble(double val);
OF_CORE_EXPORT_API OF_Variant *OF_Variant_CreateInt64(OFInt64 val);
OF_CORE_EXPORT_API OF_Variant *OF_Variant_CreateBool(OFBool val);
OF_CORE_EXPORT_API OF_Variant *OF_Variant_CreateString(const char *str);
OF_CORE_EXPORT_API OF_Variant *OF_Variant_CreateArray();
OF_CORE_EXPORT_API OF_Variant *OF_Variant_CreateObject();
OF_CORE_EXPORT_API OF_Variant *OF_Variant_CreateBinary(OF_Binary val, void *ctx, void (*onFree)(void *ctx, OFUInt8 *data));
OF_CORE_EXPORT_API OF_VariantType OF_Variant_GetType(OF_Variant *ref);
OF_CORE_EXPORT_API double OF_Variant_ToDouble(OF_Variant *ref);
OF_CORE_EXPORT_API OFInt64 OF_Variant_ToInt64(OF_Variant *ref);
OF_CORE_EXPORT_API OFBool OF_Variant_ToBool(OF_Variant *ref);
OF_CORE_EXPORT_API const char *OF_Variant_ToString(OF_Variant *ref);
OF_CORE_EXPORT_API OF_Binary OF_Variant_ToBinary(OF_Variant *ref);

OF_CORE_EXPORT_API void OF_Variant_Retain(OF_Variant *ref);
OF_CORE_EXPORT_API void OF_Variant_Release(OF_Variant *ref);

OF_CORE_EXPORT_API OFInt32 OF_Variant_GetSize(OF_Variant *ref);

// Object
OF_CORE_EXPORT_API OF_Variant *OF_Variant_Find(OF_Variant *ref, const char *key);
OF_CORE_EXPORT_API void OF_Variant_Insert(OF_Variant *ref, const char *key, OF_Variant *val);

// Array
OF_CORE_EXPORT_API OF_Variant *OF_Variant_At(OF_Variant *ref, OFInt32 index);
OF_CORE_EXPORT_API void OF_Variant_Append(OF_Variant *ref, OF_Variant *val);

OF_CORE_EXPORT_API OF_StringHolder *OF_Variant_TransToJsonString(OF_Variant *ref, OFBool isPretty, OFBool withBinary);

#ifdef __cplusplus
}
#endif

#endif // _OF_CORE_H_
