#ifndef _VN2_COMMON_H_
#define _VN2_COMMON_H_


#ifdef __cplusplus
extern "C" {
#endif


#define VN2_NULL                             0
#define VN2_TRUE                             1
#define VN2_FALSE                            0


/* ------------------------------------------- */
/* Base type definition                		   */

#if defined(VN_WIN32) || defined(_WIN32) || defined(WIN32)
typedef __int8                          VN2Int8;
typedef __int16                         VN2Int16;
typedef __int32                         VN2Int32;
typedef __int64                         VN2Int64;
typedef unsigned __int8                 VN2UInt8;
typedef unsigned __int16                VN2UInt16;
typedef unsigned __int32                VN2UInt32;
typedef unsigned __int64                VN2UInt64;
#ifdef _WIN64
typedef unsigned __int64                VN2UIntPtr;
typedef __int64                         VN2IntPtr;
#else  /* _WIN64 */
typedef __w64 unsigned int              VN2UIntPtr;
typedef __w64 int                       VN2IntPtr;
#endif  /* _WIN64 */
#else
#include <stdint.h>
typedef int8_t                          VN2Int8;
typedef int16_t                         VN2Int16;
typedef int32_t                         VN2Int32;
typedef int64_t                         VN2Int64;
typedef uint8_t                         VN2UInt8;
typedef uint16_t                        VN2UInt16;
typedef uint32_t                        VN2UInt32;
typedef uint64_t                        VN2UInt64;
typedef uintptr_t                       VN2UIntPtr;
typedef intptr_t                        VN2IntPtr;
typedef void                            VN2Void;
#endif
typedef void* 							VN2VoidPtr;
typedef int                             VN2Bool;
typedef char                            VN2Path[1024];
typedef char                            VN2Text[1024];
typedef char                            VN2Name[256];
typedef char                            VN2UUID[64];
typedef VN2UInt32                       VN2Handle;
typedef float                           VN2Float32;
typedef double                          VN2Float64;

/* ------------------------------------------- */

/* ------------------------------------------- */
/* Global API result definition                */

typedef enum _VN2_Result {
    VN2_Result_Success 				=  	 0,
    VN2_Result_Failed				= 	-1,
    VN2_Result_NotInit				= 	-2,
    VN2_Result_InvalidInput			= 	-3,
    VN2_Result_InvalidFrameHandler	= 	-4,
    VN2_Result_Failed_GLVersion		= 	-5,
    VN2_Result_Failed_GLEnvironment	= 	-6,
} VN2_Result;

/* ------------------------------------------- */
/* Global Log level definition                 */
typedef enum _VN2_LogLevel {
    VN2_LOG_LEVEL_VERBOSE = 0x00000001, // 0b 0...00000001
    VN2_LOG_LEVEL_DEBUG =   0x00000002, // 0b 0...00000010
    VN2_LOG_LEVEL_INFO =    0x00000004, // 0b 0...00000100
    VN2_LOG_LEVEL_WARN =    0x00000008, // 0b 0...00001000
    VN2_LOG_LEVEL_ERROR =   0x00000010, // 0b 0...00010000
    VN2_LOG_LEVEL_ALL =     0x000000ff, // 0b 0...11111111 (Easy setting to enable all log-level informations)
} VN2_LogLevel;

/* ------------------------------------------- */


/* ------------------------------------------- */
/* Some base interface definition.             */

typedef enum _VN2_PIX_FMT {
    VN2_PIX_FMT_UNKNOW,      /*  Unknow pixel format, as a cube */
    VN2_PIX_FMT_YUVI420,     /*  YUV  4:2:0   12bpp ( 3 planes, the first is Y, the second is U, the third is V */
    VN2_PIX_FMT_YUV420F,     /*  YUV  4:2:0   12bpp ( 2 planes, ios FullRange, the first is Y - luminance channel, the other channel was UV alternate permutation.) */
    VN2_PIX_FMT_YUV420V,     /*  YUV  4:2:0   12bpp ( 2 planes, ios VideoRange, the first is Y - luminance channel, the other channel was UV alternate permutation.) */
    VN2_PIX_FMT_YUV420P_888_SKIP1, /*  YUV  4:2:0   12bpp ( 3 planes, android Camera2, the first is Y - luminance channel, the second is U channel with skip 1, the third is V channel with skip 1.) */
    VN2_PIX_FMT_BGRA8888,    /*  BGRA 8:8:8:8 32bpp ( 4 channel, 8x4=32bit BGRA pixel ) */
    VN2_PIX_FMT_RGBA8888,    /*  RGBA 8:8:8:8 32bpp ( 4 channel, 8x4=32bit RGBA pixel ) */
    VN2_PIX_FMT_GRAY8,       /*  Y    1        8bpp ( 1 channel, 8bit luminance pixel ) */
    VN2_PIX_FMT_NV12,        /*  YUV  4:2:0   12bpp ( 2 planes, the first is Y - luminance channel, the other channel was UV alternate permutation ) */
    VN2_PIX_FMT_NV21,        /*  YUV  4:2:0   12bpp ( 2 planes, andoird default, the first is Y - luminance channel, the other channel was VU alternate permutation ) */
    VN2_PIX_FMT_BGR888,      /*  BGR  8:8:8   24bpp ( 3 channel, 8x3=24bit BGR pixel ) */
    VN2_PIX_FMT_RGB888,      /*  RGB  8:8:8   24bpp ( 3 channel, 8x3=24bit RGB pixel ) */
    VN2_PIX_FMT_GRAY32,      /*  Y    1        8bpp ( 1 channel, 32bit float luminance pixel ) */
    VN2_PIX_FMT_CHW_U8,      /*  As a cube , data layerout was chw, data type was unsigned char */
    VN2_PIX_FMT_CHW_F32,     /*  As a cube , data layerout was chw, data type was float 32 */
    VN2_PIX_FMT_ERROR        /*  Error pixel format */
} VN2_PIX_FMT;

typedef enum _VN2_ORIENT_FMT {
    /*
     Like value as VN2_ORIENT_FMT_ROTATE_90L|VN2_ORIENT_FMT_FLIP_V, means fristly anticlockwise rotate 90 degree, and then flip vertically;
     So, the priority rotation is higher then flip.
     */
    VN2_ORIENT_FMT_DEFAULT =     0x00000000,	/*  Unknow orientated format, as a default option, no rotate and no flip */
    VN2_ORIENT_FMT_ROTATE_90L =  0x00000001, /*  anticlockwise rotate 90 degree (clockwise rotate 270 degree)  */
    VN2_ORIENT_FMT_ROTATE_90R =  0x00000002, /*  clockwise rotate 90 degree  */
    VN2_ORIENT_FMT_ROTATE_180 =  0x00000004, /*  rotate 180 degree  */
    VN2_ORIENT_FMT_FLIP_V =      0x00000008, /*  flip vertically */
    VN2_ORIENT_FMT_FLIP_H =      0x00000010, /*  flip horizontally */
    VN2_ORIENT_FMT_ROTATE_360 =  0x00000020, /*  android case: post carma orientation = 270 degree */
    VN2_ORIENT_FMT_ROTATE_180L = 0x00000040, /*  android case: post carma orientation = 270 degree */
    VN2_ORIENT_FMT_ERROR =       0xFFFFFFFF, /*  ERROR */
} VN2_ORIENT_FMT;

typedef enum _VN2_MODE_FMT {
    VN2_MODE_FMT_VIDEO = 0x00000000,
    VN2_MODE_FMT_PICTURE = 0x00000001,
    VN2_MODE_FMT_DEFAULT = 0x00000000,
} VN2_MODE_FMT;

typedef struct _VN2_Texture {
    VN2UInt32 	textureID;	/* OpenGL texture id.*/
    VN2UInt32 	target;    	/* OpenGL texture target, e.g. GL_TEXTURE_2D.*/
    VN2UInt32 	format;    	/* OpenGL texture format, e.g. GL_RGBA.*/
    VN2Int32 	width;		/* OpenGL texture width.*/
    VN2Int32 	height;		/* OpenGL texture height.*/
} VN2_Texture;

typedef struct _VN2_Rect2D {
    VN2Float32 x0;	/* left 	*/
    VN2Float32 y0; 	/* top 		*/
    VN2Float32 x1; 	/* right 	*/
    VN2Float32 y1; 	/* bottom	*/
} VN2_Rect2D;

typedef struct _VN2_Point2D {
    VN2Float32 x;
    VN2Float32 y;
} VN2_Point2D;

typedef struct _VN2_Image {
    VN2_ORIENT_FMT   ori_fmt;	 /* orientation format enum of img*/
    VN2_PIX_FMT      pix_fmt;    /* pixel format enum of img */
    VN2_MODE_FMT	 mode_fmt;	 /* mode format enum of detection, can use video/picture/default */
    VN2Int32         width;      /* width of img */
    VN2Int32         height;     /* height of img */
    VN2Int32         channels;   /* channels of img */
    VN2_Rect2D       rect;       /* rect record the place in target image where this image should be put into, when used as output this member may help.*/
    VN2VoidPtr       data;       /* if is cpu-backend, data means the raw data of img, default type is "unsigned char". But data is NULL in gpu-backend. */
    VN2VoidPtr       texture;    /* if is cpu-backend, texture is NULL. But if is gpu-backend, texture may be a VN2_Texture(android|ios) or a CVPixelbuffer(ios). */
} VN2_Image;



/* ------------------------------------------- */

/* ------------------------------------------- */
/* Some definition of Object Count.            */
typedef struct _VN2_ObjectCountDataArr {
    VN2Int32     count;
    VN2_Rect2D * objRectArr;
} VN2_ObjCountDataArr;
void VN2_ObjCountDataArr_Free(VN2_ObjCountDataArr *);
/* ------------------------------------------- */



/* ------------------------------------------- */
/* Some definition of Frame mask		       */
typedef struct _VN2_FrameInstanceMaskData {
    VN2Int32	width;
    VN2Int32    height;
    VN2_Rect2D  maskRect;
    VN2VoidPtr  data;
}VN2_FrameInstanceMaskData;


/* ------------------------------------------- */
/* Some Face definition.             		   */

#define VN2_FRAMEDATA_MAX_FACE_LANDMARKS_NUM 278
#define VN2_FRAMEDATAARR_MAX_FACES_NUM       5

typedef struct _VN2_FaceFrameData {
    VN2Float32 	    faceScore;
    VN2_Rect2D 	    faceRect;
    VN2UInt32       inputWidth;
    VN2UInt32       inputHeight;
    VN2UInt32 	    faceLandmarksNum;
    VN2_Point2D 	faceLandmarks[VN2_FRAMEDATA_MAX_FACE_LANDMARKS_NUM];
    VN2Float32 	    faceLandmarkScores[VN2_FRAMEDATA_MAX_FACE_LANDMARKS_NUM];
    /* Face Attributes */
    VN2Bool         ifCloseLeftEye;
    VN2Bool         ifCloseRightEye;
    VN2Bool 		ifBlinkLeftEye;
    VN2Bool 		ifBlinkRightEye;
    VN2Bool 		ifOpenMouth;
    VN2Bool 		ifShakeHead;
    VN2Bool 		ifNodHead;
    VN2Bool         ifOpenCloseMouth;
    VN2Float32      smileScore;
    VN2Float32      faceYaw;
} VN2_FaceFrameData;

typedef struct _VN2_FaceFrameDataArr {
    VN2UInt32 			facesNum;
    VN2_FaceFrameData 	facesArr[VN2_FRAMEDATAARR_MAX_FACES_NUM];
} VN2_FaceFrameDataArr;

typedef struct _VN2_FaceMaskDataArr {
    VN2UInt32 			        facesNum;
    VN2_FrameInstanceMaskData 	faceMaskArr[VN2_FRAMEDATAARR_MAX_FACES_NUM];
} VN2_FaceMaskDataArr;

#define VN2_FACE_APPLY_VIDEO_MODE 	0
#define VN2_FACE_APPLY_PICTURE_MODE 	1

/* ------------------------------------------- */


/* ------------------------------------------- */
/* Some Gesture definition.             	   */

#define VN2_FRAMEDATAARR_MAX_GESTURE_NUM  15

typedef enum _VN2_GestureType {
    VN2_GestureType_Unknow       = 0x00,
    VN2_GestureType_V            = 0x01, // Pinyin: jian dao shou
    VN2_GestureType_ThumbUp      = 0x02, // Pinyin: dian zan
    VN2_GestureType_OneHandHeart = 0x03, // Pinyin: dan shou bi xin
    VN2_GestureType_SpiderMan    = 0x04, // Pinyin: zhi zhu xia
    VN2_GestureType_Lift         = 0x05, // Pinyin: tuo ju
    VN2_GestureType_666          = 0x06, // 6 6 6
    VN2_GestureType_TwoHandHeart = 0x07, // Pinyin: shuang shou bi xin
    VN2_GestureType_PalmTogether = 0x08, // Pinyin: bao quan
    VN2_GestureType_PalmOpen     = 0x09, // Pinyin: zhang kai shou zhang
    VN2_GestureType_ZuoYi        = 0x0a, // Pinyin: zuo yi | bao quan
    VN2_GestureType_OK           = 0x0b, // OK
    VN2_GestureType_ERROR        = 0xff,
} VN2_GestureType;

typedef struct _VN2_GestureFrameData {
    VN2_GestureType 	type;                   /* Gesture type */
    VN2_Rect2D 		rect;					/* Gesture rect, left-top-right-bottom */
    VN2Float32		score;					/* Gesture confidence socre */
} VN2_GestureFrameData;

typedef struct _VN2_GestureFrameDataArr {
    VN2UInt32 			gestureNum;
    VN2_GestureFrameData gestureArr[VN2_FRAMEDATAARR_MAX_GESTURE_NUM];
} VN2_GestureFrameDataArr;

/* ------------------------------------------- */


/* ------------------------------------------- */
/* Some struct definitions for pose sdk.       */

#define VN2_FRAMEDATA_MAX_BODYLANDMARKS_NUM  22
#define VN2_FRAMEDATAARR_MAX_BODYS_NUM       5
typedef enum _VN2_BodyResultDesc {
    VN2_BodyResultDesc_Normal = 0,
    VN2_BodyResultDesc_NoPerson = 1,						// "没有人"
    VN2_BodyResultDesc_MorethanOnePerson = 2,			// "注意：仅支持1人"
    VN2_BodyResultDesc_NoKneeSeen = 3,					// "看不到膝盖，请退后"
    VN2_BodyResultDesc_NoFootSeen = 4,					// "看不到脚部，请退后"
    VN2_BodyResultDesc_NoHipSeen = 5                     // "看不到腰部，请退后"
}VN2_BodyResultDesc;
typedef struct _VN2_BodyFrameData {
    VN2Float32   bodyScore;
    VN2_Rect2D   bodyRect;
    VN2UInt32    bodyLandmarksNum;
    VN2_Point2D  bodyLandmarks[VN2_FRAMEDATA_MAX_BODYLANDMARKS_NUM];
    VN2Float32   bodyLandmarkScores[VN2_FRAMEDATA_MAX_BODYLANDMARKS_NUM];
    VN2UInt32	 isWriggleWaist;
    VN2UInt32	 isCrouch;
    VN2UInt32	 isRun;
    VN2_BodyResultDesc bodyResultDesc;
} VN2_BodyFrameData;

typedef struct _VN2_BodyFrameDataArr {
    VN2UInt32         bodiesNum;
    VN2_BodyFrameData bodiesArr[VN2_FRAMEDATAARR_MAX_BODYS_NUM];
} VN2_BodyFrameDataArr;

/* ------------------------------------------- */

/* ---------------------------------------------------------- */
/* Some struct definitions for hand reconstruction sdk.       */

#define VN2_HANDRECONSTRUCTION_SHAPE_COEF_DIMS 10
#define VN2_HANDRECONSTRUCTION_POSE_COEF_DIMS  33
#define VN2_HANDLECONSTRUCTION_CAM_COEF_DIMS   3


typedef struct _VN2_HandReconstructionFrameData {
    VN2Float32 rightHandShapeCoefs[VN2_HANDRECONSTRUCTION_SHAPE_COEF_DIMS];
    VN2Float32 rightHandPoseCoefs[VN2_HANDRECONSTRUCTION_POSE_COEF_DIMS];
    VN2Float32 rightHandCamCoefs[VN2_HANDLECONSTRUCTION_CAM_COEF_DIMS];
    VN2Float32 leftHandShapeCoefs[VN2_HANDRECONSTRUCTION_SHAPE_COEF_DIMS];
    VN2Float32 leftHandPoseCoefs[VN2_HANDRECONSTRUCTION_POSE_COEF_DIMS];
    VN2Float32 leftHandCamCoefs[VN2_HANDLECONSTRUCTION_CAM_COEF_DIMS];
} VN2_HandReconstructionFrameData;

/* ---------------------------------------------------------- */


/* ---------------------------------------------------------- */
/* Some struct definitions for body reconstruction sdk.       */

#define VN2_BODYRECONSTRUCTION_POSE_COEF_DIMS 207
#define VN2_BODYRECONSTRUCTION_SHAPE_COEF_DIMS 10
#define VN2_BODYRECONSTRUCTION_GLOBAL_ROTATE_COEF_DIMS  9
#define VN2_BODYLECONSTRUCTION_CAM_OFFSET_COEF_DIMS 3  


typedef struct _VN2_BodyReconstructionFrameData {
    VN2Float32 bodyPoseCoefs[VN2_BODYRECONSTRUCTION_POSE_COEF_DIMS];
    VN2Float32 bodyShapeCoefs[VN2_BODYRECONSTRUCTION_SHAPE_COEF_DIMS];
    VN2Float32 bodyGlobalRotateCoefs[VN2_BODYRECONSTRUCTION_GLOBAL_ROTATE_COEF_DIMS];
    VN2Float32 bodyCamOffsetCoefs[VN2_BODYLECONSTRUCTION_CAM_OFFSET_COEF_DIMS];
} VN2_BodyReconstructionFrameData;

/* ---------------------------------------------------------- */

/* ------------------------------------------- */
/* Some definition of Classification.          */
#define VN2_CLASSIFICATION_ACC_TOP_N 5
#define VN2_MAX_MULTI_CLASSFICATION_NUM 10
#define VN2_MAX_LABEL_LENGTH 100
typedef struct _VN2_Classification_Accuracy_Top_N {
    VN2UInt32  categories[VN2_CLASSIFICATION_ACC_TOP_N];
    char   labels[VN2_CLASSIFICATION_ACC_TOP_N][VN2_MAX_LABEL_LENGTH];
    VN2Float32 probabilities[VN2_CLASSIFICATION_ACC_TOP_N];
    VN2Int32 usedTopN;
} VN2_ClsTopNAcc;

typedef struct _VN2_ClassificationTopNDataArr {
    VN2UInt32 			numCls;
    VN2_ClsTopNAcc      clsArr[VN2_MAX_MULTI_CLASSFICATION_NUM];

} VN2_ClsTopNAccArr;

typedef struct _VN2_MultiClassificationTopNDataArr {
    VN2UInt32 			numOut;
    VN2_ClsTopNAccArr      multiClsArr[VN2_FRAMEDATAARR_MAX_FACES_NUM]; //used for multi face

} VN2_MultiClsTopNAccArr;
/* ------------------------------------------- */

typedef struct _VN2_ImageArr {
    VN2Int32     imgsNum;
    VN2_Image    imgsArr[VN2_FRAMEDATAARR_MAX_FACES_NUM];
}VN2_ImageArr;

#ifdef __cplusplus
}
#endif

#endif // _VN2_COMMON_H_
