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
#ifndef _OF_EFFECT_H_
#define _OF_EFFECT_H_

#include "of_core.h"

#ifdef __cplusplus
extern "C" {
#endif

/************************************************************************/
/* Filter Type                                                          */
/************************************************************************/
#define KFilterCopy                     "CopyFilter"
#define KFilterMirror                   "MirrorFilter"
#define KFilterNV12toRGB                "NV12toRGB"
#define KFilterBeauty0                  "BeautyFilter0"
#define KFilterBeauty1                  "BeautyFilter1"
#define KFilterBeauty2                  "BeautyFilter2"
#define KFilterBeauty3                  "BeautyFilter3"
#define KFilterBeauty4                  "BeautyFilter4"
#define KFilterBeauty5                  "BeautyFilter5"
#define KFilterBeauty6                  "BeautyFilter6"
#define KFilterBeauty7                  "BeautyFilter7"
#define KFilterBeauty8                  "BeautyFilter8"
#define KFilterBeautyFaceU              "BeautyFaceUFilter"
#define KFilterDenoise                  "Denoise"
#define KFilterBlend                    "BlendFilter"
#define KFilterPaint                    "PaintFilter"
#define KFilterBlendAnimation           "BlendAnimationFilter"
#define KFilterSoftGlow                 "SoftGlowFilter"
#define KFilterMattingBlend             "MattingBlendFilter"
#define KFilterMaskBlur                 "MaskBlurFilter"
#define KFilterFixedStickersAnimation   "FixedStickersAnimationFilter"
#define KFilterDynStickersAnimation     "DynStickersAnimationFilter"
#define KFilterCurve                    "CurveFilter"
#define KFilterSelectiveColor           "SelectiveColorFilter"
#define KFilterZoom                     "ZoomFilter"
//#define KFilterThinFace                 "ThinFaceFilter"
#define KFilterFaceLifting              "FaceLiftingFilter"
#define KFilterFaceLiftingOpt           "FaceLiftingOptFilter"
#define KFilterMTFaceLiftingOpt         "MTFaceLiftingOptFilter"
#define KFilterComiclFaceLifting        "ComiclFaceLiftingFilter"
#define KFilterBasicThinFace            "BasicThinFaceFilter"
#define KFilteeI18nFaceLifting          "I18nFaceLiftingFilter"
#define KFilterBigHead                  "BigHeadFilter"
#define KFilterFleshyFace               "FleshyFaceFilter"
#define KFilterFace2dMask               "Face2dMaskFilter"
#define KFilterFacialMask               "FacialMaskFilter"
#define KFilterFacialSticker            "FacialStickerFilter"
#define KFilterFacialMaskAnimation      "FacialMaskAnimationFilter"
#define KFilterFaceSegment              "FaceSegmentFilter"
#define KFilterFace2dAnimation          "Face2dAnimationFilter"
#define KFilterLookUpTableAnimation     "LookUpTableAnimationFilter"
#define KFilterEdgeLookUpTableAnimation "EdgeLookUpTableAnimationFilter"
#define KFilterLookUpTable              "LookUpTableFilter"
#define KFilterLookUpTable3D            "LookUpTable3DFilter"
#define KFilterMaskLookUpTable          "MaskLookUpTableFilter"
#define KFilterBeautyMakeupFilter0      "BeautyMakeupFilter0"
#define KFilterBeautyMakeupFilter       "BeautyMakeupFilter"
#define KFilterFaceSwap                 "FaceSwapFilter"
#define KFilterTTPTFaceSwap             "TTPTFaceSwapFilter"
#define KFilterMultiFaceSwap            "MultiFaceSwapFilter"
#define KFilterSobelEdgeDetection       "SobelEdgeDetectionFilter"
#define KFilterSobelColorEdge           "SobelColorEdgeFilter"
#define KFilterDirectionalSobelEdgeDetection    "DirectionalSobelEdgeDetectionFilter"
#define KFilterEdgeMaskLookUpTable      "EdgeMaskLookUpTableFilter"
#define KFilterRobertsEdgeDetection     "RobertsEdgeDetectionFilter"
#define KFilterLaplacianEdgeDetection   "LaplacianEdgeDetectionFilter"
#define KFilterCannyEdgeDetection       "CannyEdgeDetectionFilter"
#define KFilterHighLightShadow          "HighLightShadowFilter"
#define KFilterHighLightShadowTint      "HighLightShadowTintFilter"
#define KFilterSketch                   "SketchFilter"
#define KFilterShake                    "ShakeFilter"
#define KFilterRandomShake              "RandomShakeFilter"
#define KFilterSoulObe                  "SoulObeFilter"
#define KFilterSoulBlend                "SoulBlendFilter"
#define KFilterSoulTrail                "SoulTrailFilter"
#define KFilterPhantom                  "PhantomFilter"
#define KFilterDivision                 "DivisionFilter"
//#define KFilterThresholdSketch          "ThresholdSketchFilter"
#define KFilterToon                     "ToonFilter"
#define KFilterSmoothToon               "SmoothToonFilter"
#define KFilterFaceRangeBlur            "FaceRangeBlurFilter"
#define KFilterMotionBlur               "MotionBlurFilter"
#define KFilterZoomBlur                 "ZoomBlurFilter"
#define KFilterSpiralBlur               "SpiralBlurFilter"
#define KFilterTranslationBlur          "TranslationBlurFilter"
#define KFilterBokehBlur                "BokehBlurFilter"
#define KFilterUnsharpMask              "UnsharpMaskFilter"
#define KFilterDelayPlay                "DelayPlayFilter"
#define KFilterDelayPause               "DelayPauseFilter"
#define KFilterNoiseTV                  "NoiseTVFilter"
#define KFilterDistortionShake          "DistortionShakeFilter"
#define KFilterVibration                "VibrationFilter"
#define KFilterBrowse                   "BrowseFilter"
#define KFilterComponentMove            "ComponentMoveFilter"
#define KFilterRandNoise                "RandNoiseFilter"
//#define KFilterSlices                   "SlicesFilter"
#define KFilterRipple                   "RippleFilter"
//#define KFilterHoneycomb                "HoneycombFilter"
//#define KFilterWave                     "WaveFilter"
#define KFilterFrameStamp               "FrameStampFilter"
//#define KFilterKuwahara                 "KuwaharaFilter"
//#define KFilterMosaic                   "MosaicFilter"
#define KFilterPixellate                "PixellateFilter"
#define KFilterPixellatePosition        "PixellatePositionFilter"
#define KFilterPolarPixellate           "PolarPixellateFilter"
//#define KFilterCrosshatch               "CrosshatchFilter"
#define KFilterColorPacking             "ColorPackingFilter"
#define KFilterVignette                 "VignetteFilter"
//#define KFilterSwirl                    "SwirlFilter"
//#define KFilterBulgeDistortion          "BulgeDistortionFilter"
//#define KFilterPinchDistortion          "PinchDistortionFilter"
#define KFilterStretchDistortion        "StretchDistortionFilter"
#define KFilterGlassSphere              "GlassSphereFilter"
//#define KFilterSphereRefraction         "SphereRefractionFilter"
//#define KFilterDilation                 "DilationFilter"
#define KFilterHarrisCornerDetection    "HarrisCornerDetectionFilter"
//#define KFilterPosterize                "PosterizeFilter"
//#define KFilterCGAColorspace            "CGAColorspaceFilter"
//#define KFilterPerlinNoise              "PerlinNoiseFilter"
//#define KFilterConvolution3x3           "Convolution3x3Filter"
//#define KFilterEmboss                   "EmbossFilter"
//#define KFilterPolkaDot                 "PolkaDotFilter"
//#define KFilterHalftone                 "HalftoneFilter"
#define KFilterFace3dMesh               "Face3dMeshFilter"
#define KFilterFace3dPhysicsMesh        "Face3dPhysicsMeshFilter"
#define KFilterFace3dPlaneAnimation     "Face3dPlaneAnimationFilter"
#define KFilterTrackPlaneAnimation      "TrackPlaneAnimationFilter"
#define KFilterTrackText                "TrackTextFilter"
#define KFilterBasketballPhysics        "BasketballPhysicsFilter"
#define KFilterGameFood                 "GameFoodFilter"
#define KFilterFaceParticle             "FaceParticleFilter"
#define KFilterScreenParticle           "ScreenParticleFilter"
#define KFilterTrackParticle            "TrackParticleFilter"
#define KFilterTrackParticleSystem      "TrackParticleSystemFilter"
#define KFilterPathParticle             "PathParticleFilter"
#define KFilter3DAR                     "Ar3dfilter"
#define KFilterARParticle               "ArParticleFilter"
#define KFilterFixedStickersSVGA        "FixedStickersSVGAFilter"
#define KFilterMirror0                  "MirrorFilter0"
//#define KFilterFireCircle               "FireCircleFilter"
#define KFilterPupilSwap                "PupilSwapFilter"
#define KFilterSvga2Animation           "Svga2filter"
#define KFilterCustomLua                "CustomLuaFilter"
#define KFilterCustomLua2               "CustomLuaFilter2"
#define KFilterGreycstoration           "GreycstorationFilter"

/************************************************************************/
/* Animator Type                                                        */
/************************************************************************/
#define KAnimatorFilterFloatParam       "FilterFloatParamAnimator"
#define KAnimatorViewportsSceneParam    "ViewportsSceneParamAnimator"
#define KAnimatorAudioParam             "AudioParamAnimator"

/************************************************************************/
/* Scene Type                                                           */
/************************************************************************/
#define KSceneViewports                 "ViewportsScene"
#define KSceneSegment                   "SegmentScene"
#define KSceneClone                     "CloneScene"
#define KFaceSegmentScene               "FaceSegmentScene"
#define KSceneMultiFace                 "MultiFaceScene"
#define KSceneGesture                   "GestureScene"
#define KSceneGestureTrack              "GestureTrackScene"
#define kSceneHands                     "HandsScene"
#define KSceneStamp                     "StampScene"

/************************************************************************/
/* Game Type                                                            */
/************************************************************************/
#define KGameEmpty                      "EmptyGame"
#define KGameBasketball                 "BasketballGame"
#define KGameFood                       "FoodGame"
#define KGameCustomLua                  "CustomLuaGame"
#define KGamePoseMatch                  "PoseMatchGame"


/************************************************************************/
/* Graphics Node Type                                                            */
/************************************************************************/
#define KGraphicsNodeCacheLastTexNode       "CacheLastTexNode"
#define KGraphicsNodeFilterNode             "FilterNode"
#define KGraphicsNodeGlobalParamNode        "GlobalParamNode"
#define KGraphicsNodeMathFourParamFunNode   "MathFourParamFunNode"
#define KGraphicsNodeMathOneParamFunNode    "MathOneParamFunNode"
#define KGraphicsNodeMathOperNode           "MathOperNode"
#define KGraphicsNodeMathThreeParamFunNode  "MathThreeParamFunNode"
#define KGraphicsNodeMeshNode               "MeshNode"
#define KGraphicsNodeNodeProgramCodeNode    "NodeProgramCodeNode"
#define KGraphicsNodeNodeProgramNode        "NodeProgramNode"
#define KGraphicsNodeNumberNode             "NumberNode"
#define KGraphicsNodePathProgramNode        "PathProgramNode"
#define KGraphicsNodeProgramNode            "ProgramNode"
#define KGraphicsNodeTexSamplerNode         "TexSamplerNode"
#define KGraphicsNodeTransformNode          "TransformNode"


/************************************************************************/
/* Filter's floating type parameter definition.                         */
/************************************************************************/
typedef enum _OF_ParamFilterBeauty0
{
    OF_ParamFilterBeauty0_BeautyIntensity = 0,
    OF_ParamFilterBeauty0_Whiteness
} OF_ParamFilterBeauty0;

typedef enum _OF_ParamFilterBeauty1
{
    OF_ParamFilterBeauty1_BeautyIntensity = 0,
    OF_ParamFilterBeauty1_Whiteness
} OF_ParamFilterBeauty1;

typedef enum _OF_ParamFilterBeauty2
{
    OF_ParamFilterBeauty2_BeautyIntensity = 0,
    OF_ParamFilterBeauty2_Whiteness
} OF_ParamFilterBeauty2;

typedef enum _OF_ParamFilterBeauty3
{
    OF_ParamFilterBeauty3_BeautyIntensity = 0,
    OF_ParamFilterBeauty3_Whiteness
} OF_ParamFilterBeauty3;

typedef enum _OF_ParamFilterBeauty4
{
    OF_ParamFilterBeauty4_BeautyIntensity = 0,
    OF_ParamFilterBeauty4_Whiteness
} OF_ParamFilterBeauty4;

typedef enum _OF_ParamFilterBeauty5
{
    OF_ParamFilterBeauty5_BeautyIntensity = 0,
    OF_ParamFilterBeauty5_Whiteness
} OF_ParamFilterBeauty5;

typedef enum _OF_ParamFilterThinFace
{
    OF_ParamFilterThinFace_ThinIntensity = 0
} OF_ParamFilterThinFace;

typedef enum _OF_ParamFilterFaceLifting
{
    OF_ParamFilterFaceLifting_ThinIntensity = 0,
    OF_ParamFilterFaceLifting_SmallfaceIntensity = 5,
    OF_ParamFilterFaceLifting_BigEyeIntensity = 8,
    OF_ParamFilterFaceLifting_EyeOffestIntensity = 11,
    OF_ParamFilterFaceLifting_NoseIntensity = 12,
    OF_ParamFilterFaceLifting_MouthIntensity = 15,
    OF_ParamFilterFaceLifting_SquashedFaceIntensity = 18,
    OF_ParamFilterFaceLifting_ChinLiftingIntensity = 21,
    OF_ParamFilterFaceLifting_ForeheadLiftingIntensity = 24,
    OF_ParamFilterFaceLifting_LongNoseIntensity = 27,
    OF_ParamFilterFaceLifting_WideForeheadLiftingIntensity = 30,
    OF_ParamFilterFaceLifting_YYMixerNoseIntensity = 33
} OF_ParamFilterFaceLifting;

typedef enum _OF_ParamFilterFaceLiftingOPt
{
    OF_ParamFilterFaceLiftingOpt_MeshCol = 0,
    OF_ParamFilterFaceLiftingOpt_ThinIntensity = 1,
    OF_ParamFilterFaceLiftingOpt_SmallfaceIntensity = 6,
    OF_ParamFilterFaceLiftingOpt_SquashedFaceIntensity= 9,
    OF_ParamFilterFaceLiftingOpt_ForeheadLiftingIntensity = 12,
    OF_ParamFilterFaceLiftingOpt_WideForeheadLiftingIntensity = 15,
    OF_ParamFilterFaceLiftingOpt_BigSmallEyeIntensity = 18,
    OF_ParamFilterFaceLiftingOpt_EyeOffestIntensity = 20,
    OF_ParamFilterFaceLiftingOpt_EyesRotationIntensity = 21,
    OF_ParamFilterFaceLiftingOpt_ThinNoseIntensity = 23,
    OF_ParamFilterFaceLiftingOpt_LongNoseIntensity = 26,
    OF_ParamFilterFaceLiftingOpt_ThinNoseBridgeIntensity = 29,
    OF_ParamFilterFaceLiftingOpt_ThinMouthIntensity = 32,
    OF_ParamFilterFaceLiftingOpt_MovemouthIntensity = 34,
    OF_ParamFilterFaceLiftingOpt_ChinLiftingIntensity = 37,

} OF_ParamFilterFaceLiftingOpt;

typedef enum _OF_ParamFilterMTFaceLiftingOPt
{
    OF_ParamFilterMTFaceLiftingOpt_MeshCol = 0,
    OF_ParamFilterMTFaceLiftingOpt_ThinIntensity = 1,
    OF_ParamFilterMTFaceLiftingOpt_SquashedFaceIntensity = 4,
    OF_ParamFilterMTFaceLiftingOpt_BigEyeIntensity = 7,
} OF_ParamFilterMTFaceLiftingOpt;

typedef enum _OF_ParamFilterBasicThinFace
{
    OF_ParamFilterBasicThinFace_Intensity = 0,
    OF_ParamFilterBasicThinFace_Chin,
    OF_ParamFilterBasicThinFace_BigEye
} OF_ParamFilterBasicThinFace;

typedef enum _OF_ParamFilterComicalFaceLifting
{
    OF_ParamFilterComicalFaceLifting_Index = 0
} OF_ParamFilterComicalFaceLifting;

typedef enum _OF_ParamFilterBlend
{
    OF_ParamFilterBlend_Intensity = 0
} OF_ParamFilterBlend;

typedef enum _OF_ParamFilterPaint
{
    OF_ParamFilterPaint_PenRadius = 0,
    OF_ParamFilterPaint_Opacity,
    OF_ParamFilterPaint_PenColorR,
    OF_ParamFilterPaint_PenColorG,
    OF_ParamFilterPaint_PenColorB,
} OF_ParamFilterPaint;

typedef enum _OF_ParamFilterSoftGlowBlend
{
    OF_ParamFilterSoftGlowBlend_Intensity = 0,
    OF_ParamFilterSoftGlowBlend_BlurRadius,
    OF_ParamFilterSoftGlowBlend_Warmth
} OF_ParamFilterSoftGlowBlend;

typedef enum _OF_ParamFilterBrowse
{
    OF_ParamFilterBrowse_Interval = 0
} OF_ParamFilterBrowse;

typedef enum _OF_ParamFilterFace2dMask
{
    OF_ParamFilterFace2dMask_Intensity = 0
} OF_ParamFilterFace2dMask;

typedef enum _OF_ParamFilterLookUpTable
{
    OF_ParamFilterLookUpTable_Intensity = 0
} OF_ParamFilterLookUpTable;

typedef enum _OF_ParamFilterNoiseTV
{
    OF_ParamFilterNoiseTV_Interval = 0
} OF_ParamFilterNoiseTV;

typedef enum _OF_ParamFilterSlices
{
    OF_ParamFilterSlices_Interval = 0,
    OF_ParamFilterSlices_Intensity
} OF_ParamFilterSlices;

typedef enum _OF_ParamFilterComponentMove
{
    OF_ParamFilterComponentMove_Interval = 0,
    OF_ParamFilterComponentMove_XOffset,
    OF_ParamFilterComponentMove_YOffset,
} OF_ParamFilterComponentMove;

typedef enum _OF_ParamFilterFrameStamp
{
    OF_ParamFilterFrameStamp_Xoffset = 0,
    OF_ParamFilterFrameStamp_Yoffset,
    OF_ParamFilterFrameStamp_Scale,
    OF_ParamFilterFrameStamp_WidthRatio,
    OF_ParamFilterFrameStamp_AreaRatio
} OF_ParamFilterFrameStamp;

typedef enum _OF_ParamFilterSvga2 {
    OF_ParamFilterSvga2_ImageWidth = 0,
    OF_ParamFilterSvga2_ImageHeight,
    OF_ParamFilterSvga2_XOffset,
    OF_ParamFilterSvga2_YOffset,
    OF_ParamFilterSvga2_Rotation,
    OF_ParamFilterSvga2_Speed
} OF_ParamFilterSvga2;

typedef enum _OF_ParamFilterFaceSwap
{
    OF_ParamFilterFaceSwap_BlurStep = 0,
    OF_ParamFilterFaceSwap_Opacity
} OF_ParamFilterFaceSwap;

typedef enum _OF_ParamFilterPupilSwap
{
    OF_ParamFilterPupilSwap_BlendIntensity = 0,
    OF_ParamFilterPupilSwap_RadiusIntensity,
    OF_ParamFilterPupilSwap_YOffset
} OF_ParamFilterPupilSwap;

typedef enum _OF_ParamFilterMaskLookUpTable
{
    OF_ParamFilterMaskLookUpTable_Intensity = 0
} OF_ParamFilterMaskLookUpTable;

typedef enum _OF_ParamFilterEdgeMaskLookUpTable
{
    OF_ParamFilterEdgeMaskLookUpTable_Intensity = 0,
    OF_ParamFilterEdgeMaskLookUpTable_EdgeStrength,
    OF_ParamFilterEdgeMaskLookUpTable_TexelWidth,
    OF_ParamFilterEdgeMaskLookUpTable_TexelHeight
} OF_ParamFilterEdgeMaskLookUpTable;

typedef enum _OF_ParamFilterBeautyMakeup
{
    OF_ParamFilterBeautyMakeup_Intensity = 0
} OF_ParamFilterBeautyMakeup;

typedef enum _OF_ParamFilterLookUpTableAnimation
{
    OF_ParamFilterLookUpTableAnimation_Intensity = 0
} OF_ParamFilterLookUpTableAnimation;

typedef enum _OF_ParamFilterEdgeLookUpTableAnimation
{
    OF_ParamFilterEdgeLookUpTableAnimation_Intensity = 0,
    OF_ParamFilterEdgeLookUpTableAnimation_EdgeStrength,
    OF_ParamFilterEdgeLookUpTableAnimation_TexelWidth,
    OF_ParamFilterEdgeLookUpTableAnimation_TexelHeight
} OF_ParamEdgeFilterLookUpTableAnimation;

typedef enum _OF_ParamFilterFace3dPlaneAnimation
{
    OF_ParamFilterFace3dPlaneAnimation_Size = 0,
    OF_ParamFilterFace3dPlaneAnimation_TranslateX,
    OF_ParamFilterFace3dPlaneAnimation_TranslateY,
    OF_ParamFilterFace3dPlaneAnimation_TranslateZ,
    OF_ParamFilterFace3dPlaneAnimation_Rotate,
    OF_ParamFilterFace3dPlaneAnimation_ScaleX,
} OF_ParamFilterFace3dPlaneAnimation;

typedef enum _OF_ParamFilterTrackPlaneAnimation
{
    OF_ParamFilterTrackPlaneAnimation_Width = 0,
    OF_ParamFilterTrackPlaneAnimation_Multiple,
    OF_ParamFilterTrackPlaneAnimation_TranslateX,
    OF_ParamFilterTrackPlaneAnimation_TranslateY,
    OF_ParamFilterTrackPlaneAnimation_TranslateZ,
    OF_ParamFilterTrackPlaneAnimation_Rotate,
    OF_ParamFilterTrackPlaneAnimation_ColorR,
    OF_ParamFilterTrackPlaneAnimation_ColorG,
    OF_ParamFilterTrackPlaneAnimation_ColorB,
    OF_ParamFilterTrackPlaneAnimation_ColorA
} OF_ParamFilterTrackPlaneAnimation;

typedef enum _OF_ParamFilterTrackText
{
    OF_ParamFilterTrackText_Width = 0,
    OF_ParamFilterTrackText_Height,
    OF_ParamFilterTrackText_TranslateX,
    OF_ParamFilterTrackText_TranslateY,
    OF_ParamFilterTrackText_Rotate,
    OF_ParamFilterTrackText_Text,
    OF_ParamFilterTrackText_ColorR,
    OF_ParamFilterTrackText_ColorG,
    OF_ParamFilterTrackText_ColorB,
    OF_ParamFilterTrackText_ColorA
} OF_ParamFilterTrackText;

typedef enum _OF_ParamFilterFace3dMesh
{
    OF_ParamFilterFace3dMesh_TranslateX = 0,
    OF_ParamFilterFace3dMesh_TranslateY,
    OF_ParamFilterFace3dMesh_TranslateZ,
    OF_ParamFilterFace3dMesh_RotateX,
    OF_ParamFilterFace3dMesh_RotateY,
    OF_ParamFilterFace3dMesh_RotateZ,
    OF_ParamFilterFace3dMesh_Scale,
} OF_ParamFilterFace3dMesh;

typedef enum _OF_ParamFilterAvatar
{
    OF_ParamFilterAvatar_TranslateX = 0,
    OF_ParamFilterAvatar_TranslateY,
    OF_ParamFilterAvatar_TranslateZ,
    OF_ParamFilterAvatar_RotateX,
    OF_ParamFilterAvatar_RotateY,
    OF_ParamFilterAvatar_RotateZ,
    OF_ParamFilterAvatar_Scale,
} OF_ParamFilterAvatar;

typedef enum _OF_ParamFilterParticle
{
    OF_ParamFilterParticle_TranslateX = 0,
    OF_ParamFilterParticle_TranslateY,
    OF_ParamFilterParticle_TranslateZ,
    OF_ParamFilterParticle_RotateX,
    OF_ParamFilterParticle_RotateY,
    OF_ParamFilterParticle_RotateZ,
    OF_ParamFilterParticle_Scale,
    OF_ParamFilterParticle_ColorR,
    OF_ParamFilterParticle_ColorG,
    OF_ParamFilterParticle_ColorB,
    OF_ParamFilterParticle_ColorA,

    OF_ParamFilterParticle_Count
} OF_ParamFilterParticle;

typedef enum _OF_BasketballEvent
{
    OF_BasketballEvent_Create = 1,
    OF_BasketballEvent_Destroy,
    OF_BasketballEvent_CollideBackboard,
    OF_BasketballEvent_CollideBasket,
    OF_BasketballEvent_Score,
    OF_BasketballEvent_Shoot
} OF_BasketballEvent;


typedef enum _OF_DartEvent
{
    OF_DartEvent_Create = 1,
    OF_DartEvent_Destroy,
    OF_DartEvent_CollideBackboard,
    OF_DartEvent_CollideBoard,
    OF_DartEvent_Score
} OF_DartEvent;

typedef enum _OF_ParamFilterPhysics
{
    OF_ParamFilterPhysics_Impulse,
    OF_ParamFilterPhysics_ClosingTime,
    OF_ParamFilterPhysics_MaxImpulse,
    OF_ParamFilterPhysics_MaxClosingTime,
    OF_ParamFilterPhysics_Restitution,
    OF_ParamFilterPhysics_Gravity,
    OF_ParamFilterPhysics_OpenIntensity,
    OF_ParamFilterPhysics_CloseIntensity,
    OF_ParamFilterPhysics_BasketRotX,
    OF_ParamFilterPhysics_BasketRotY,
    OF_ParamFilterPhysics_BasketRotZ,
    OF_ParamFilterPhysics_BasketTransX,
    OF_ParamFilterPhysics_BasketTransY,
    OF_ParamFilterPhysics_BasketTransZ,
    OF_ParamFilterPhysics_BasketScale,
    OF_ParamFilterPhysics_BasketStartVel,
    OF_ParamFilterPhysics_BasketMaxVel,
    OF_ParamFilterPhysics_BasketAcceler,
    OF_ParamFilterPhysics_BasketCollisionX,
    OF_ParamFilterPhysics_BasketCollisionY,
    OF_ParamFilterPhysics_BasketCollisionZ,
    OF_ParamFilterPhysics_BasketCollisionAngle,
    OF_ParamFilterPhysics_BackBoardTransZ,
    OF_ParamFilterPhysics_BallParticle,
    OF_ParamFilterPhysics_BallRotX,
    OF_ParamFilterPhysics_BallRotY,
    OF_ParamFilterPhysics_BallRotZ,
    OF_ParamFilterPhysics_BallTransX,
    OF_ParamFilterPhysics_BallTransY,
    OF_ParamFilterPhysics_BallTransZ,
    OF_ParamFilterPhysics_BallScale  /* last one */
} OF_ParamFilterPhysics;


typedef enum _OF_ParamFilterDartPhysics
{
    OF_ParamFilterDartPhysics_Impulse,
    OF_ParamFilterDartPhysics_ClosingTime,
    OF_ParamFilterDartPhysics_MaxImpulse,
    OF_ParamFilterDartPhysics_MaxClosingTime,
    OF_ParamFilterDartPhysics_Restitution,
    OF_ParamFilterDartPhysics_Gravity,
    OF_ParamFilterDartPhysics_BoardRotX,
    OF_ParamFilterDartPhysics_BoardRotY,
    OF_ParamFilterDartPhysics_BoardRotZ,
    OF_ParamFilterDartPhysics_BoardTransX,
    OF_ParamFilterDartPhysics_BoardTransY,
    OF_ParamFilterDartPhysics_BoardTransZ,
    OF_ParamFilterDartPhysics_BoardScale,
    OF_ParamFilterDartPhysics_BoardStartVel,
    OF_ParamFilterDartPhysics_BoardMaxVel,
    OF_ParamFilterDartPhysics_BoardAcceler,
    OF_ParamFilterDartPhysics_BoardCollisionX,
    OF_ParamFilterDartPhysics_BoardCollisionY,
    OF_ParamFilterDartPhysics_BoardCollisionZ,
    OF_ParamFilterDartPhysics_BoardCollisionAngle,
    OF_ParamFilterDartPhysics_BackBoardTransZ,
    OF_ParamFilterDartPhysics_DartParticle,
    OF_ParamFilterDartPhysics_DartRotX,
    OF_ParamFilterDartPhysics_DartRotY,
    OF_ParamFilterDartPhysics_DartRotZ,
    OF_ParamFilterDartPhysics_DartTransX,
    OF_ParamFilterDartPhysics_DartTransY,
    OF_ParamFilterDartPhysics_DartTransZ,
    OF_ParamFilterDartPhysics_DartScale,
    OF_ParamFilterDartPhysics_DartTorque,

    OF_ParamFilterDartPhysics_DartCreateFlag,
    OF_ParamFilterDartPhysics_DartDestroyFlag,
    OF_ParamFilterDartPhysics_DartExtParam,

    OF_ParamFilterDartPhysics_End
} OF_ParamDartFilterPhysics;

typedef enum _OF_ParamFilterFace3dPhysicsMesh
{
    OF_ParamFilter3dPhysicsMesh_FaceTransX = 0,
    OF_ParamFilter3dPhysicsMesh_FaceTransY,
    OF_ParamFilter3dPhysicsMesh_FaceTransZ,
    OF_ParamFilter3dPhysicsMesh_FaceRotX,
    OF_ParamFilter3dPhysicsMesh_FaceRotY,
    OF_ParamFilter3dPhysicsMesh_FaceRotZ,
    OF_ParamFilter3dPhysicsMesh_FaceScale,
    OF_ParamFilter3dPhysicsMesh_NoseHeight,
    OF_ParamFilter3dPhysicsMesh_FaceDepth,

    OF_ParamFilter3dPhysicsMesh_MouthTransX,
    OF_ParamFilter3dPhysicsMesh_MouthTransY,
    OF_ParamFilter3dPhysicsMesh_MouthTransZ,
    OF_ParamFilter3dPhysicsMesh_MouthXLen,
    OF_ParamFilter3dPhysicsMesh_MouthYLen,
    OF_ParamFilter3dPhysicsMesh_MouthZLen,

    OF_ParamFilter3dPhysicsMesh_CookieTransX,
    OF_ParamFilter3dPhysicsMesh_CookieTransY,
    OF_ParamFilter3dPhysicsMesh_CookieTransZ,
    OF_ParamFilter3dPhysicsMesh_CookieRotX,
    OF_ParamFilter3dPhysicsMesh_CookieRotY,
    OF_ParamFilter3dPhysicsMesh_CookieRotZ,
    OF_ParamFilter3dPhysicsMesh_CookieScale,

    OF_ParamFilter3dPhysicsMesh_PhysicsMass,
    OF_ParamFilter3dPhysicsMesh_PhysicsGravity,
    OF_ParamFilter3dPhysicsMesh_PhysicsGravityRot,
    OF_ParamFilter3dPhysicsMesh_PhysicsStiffness,
    OF_ParamFilter3dPhysicsMesh_PhysicsDamping,
    OF_ParamFilter3dPhysicsMesh_PhysicsFriction,
    OF_ParamFilter3dPhysicsMesh_PhysicsRollingFriction,
    OF_ParamFilter3dPhysicsMesh_PhysicsSpinningFriction,
    OF_ParamFilter3dPhysicsMesh_PhysicsRestitution,

    OF_ParamFilter3dPhysicsMesh_End,
} OF_ParamFilterFace3dPhysicsMesh;

typedef enum _OF_ParamFilterFoodPhysics
{
    OF_ParamFilterFoodPhysics_TransX,
    OF_ParamFilterFoodPhysics_TransY,
    OF_ParamFilterFoodPhysics_TransZ,
    OF_ParamFilterFoodPhysics_End,
} OF_ParamFilterFoodPhysics;

typedef enum _OF_ParamFilterText2d
{
    OF_ParamFilterText2d_TransX,
    OF_ParamFilterText2d_TransY,
    OF_ParamFilterText2d_Rotate,
    OF_ParamFilterText2d_Scale,
} OF_ParamFilterText2d;

typedef enum _OF_ParamFilterDynStickersAnimation
{
    OF_ParamFilterDynStickersAnimation_OffsetX,
    OF_ParamFilterDynStickersAnimation_OffsetY,
    OF_ParamFilterDynStickersAnimation_Opacity
} OF_ParamFilterDynStickersAnimation;

typedef enum _OF_ParamFilterZoom
{
    OF_ParamFilterZoom_Factor = 0
} OF_ParamFilterZoom;

/************************************************************************/
/* Global enum definition.                                              */
/************************************************************************/
typedef enum _OF_PixelFormat
{
    OF_PixelFormat_YUV420F,                 /* Support, for iphone camera(kCVPixelFormatType_420YpCbCr8BiPlanarFullRange). */
    OF_PixelFormat_YUV420V,                 /* Not support current, for iphone camera(kCVPixelFormatType_420YpCbCr8BiPlanarVideoRange).. */
    OF_PixelFormat_RGB24,                   /* Not support current. */
    OF_PixelFormat_RGB32,                   /* Support. */
    OF_PixelFormat_BGR24,                   /* Support for windows platform */
    OF_PixelFormat_BGR32,                   /* Support for windows platform */
    OF_PixelFormat_GRAY,
} OF_PixelFormat;

typedef enum _OF_BlendMode
{
    OF_BlendMode_Normal = 0,
    OF_BlendMode_Substract,
    OF_BlendMode_Multiply,
    OF_BlendMode_Overlay,
    OF_BlendMode_SoftLight,
    OF_BlendMode_HardLight,
    OF_BlendMode_LinearLight,
    OF_BlendMode_Screen,
    OF_BlendMode_ColorBurn,
    OF_BlendMode_Difference,
    OF_BlendMode_ColorDodge,
    OF_BlendMode_LinearDodge,
    OF_BlendMode_HSLColor,
    OF_BlendMode_HSLHue,
    OF_BlendMode_HSLSaturate,
    OF_BlendMode_HSLLuminosity,
    OF_BlendMode_Darken,
    OF_BlendMode_DarkenColor,
    OF_BlendMode_Lighten,
    OF_BlendMode_LightenColor,
	OF_BlendMode_RGBOverlay
} OF_BlendMode;

typedef enum _OF_FaceLandmarkMode
{
    OF_Unknown_Facelandmark = 0,
    OF_68_FaceLandmark      = 68,
    OF_106_FaceLandmark     = 106,
    OF_278_FaceLandmark     = 278,
} OF_FaceLandmarkMode;

typedef enum _OF_SoftGlowBlendMode
{
    OF_SoftGlowBlendMode_SoftGlow = 0,
    OF_SoftGlowBlendMode_SoftPortrait,
    OF_SoftGlowBlendMode_SoftLight
} OF_SoftGlowBlendMode;

typedef enum _OF_BrowseMode
{
    OF_BrowseMode_Down = 0,
    OF_BrowseMode_Up
} OF_BrowseMode;

typedef enum _OF_EdgeDetectMode
{
    OF_EdgeDetectMode_Sobel = 0,
    OF_EdgeDetectMode_Roberts,
    OF_EdgeDetectMode_Laplaciance,
    OF_EdgeDetectMode_Canny
}OF_EdgeDetectMode;

typedef enum _OF_NoiseMode
{
    OF_NoiseMode_WaveParticle = 0,
    OF_NoiseMode_Stripe
}OF_NoiseMode;

typedef enum _OF_SlicesMode
{
    OF_SlicesMode_Vertical = 0,
    OF_SlicesMode_Horizontal
}OF_SlicesMode;

typedef enum _OF_TimeVariableMode
{
    OF_TimeVariableMode_Yes = 0,
    OF_TimeVariableMode_No
}OF_TimeVariableMode;

typedef enum _OF_LocationMode
{
    OF_LocationMode_Bottom_Right = 0,
    OF_LocationMode_Top_Right,
    OF_LocationMode_Bottom_Left,
    OF_LocationMode_Top_Left,
    OF_LocationMode_Center,
    OF_LocationMode_Top_Center,
    OF_LocationMode_Bottom_Center,
    OF_LocationMode_Left_Center,
    OF_LocationMode_Right_Center
}OF_LocationMode;

typedef enum _OF_LayoutMode
{
    OF_LayoutMode_Pixel_ALL = 0,
    OF_LayoutMode_Texture_All,
    OF_LayoutMode_Image_Pixel_Edge_Texture,
    OF_LayoutMode_Image_Texture_Edge_Pixel,
    OF_LayoutMode_Image_Area_Edge_Texture
}OF_LayoutMode;

typedef enum _OF_RendererType
{
    OF_RendererType_GLES_2_0,
    OF_RendererType_GLES_3_0,
    OF_RendererType_GLES_3_1
} OF_RendererType;

typedef enum _OF_ScreenOrientation
{
    OF_ScreenOrientation_Portrait,
    OF_ScreenOrientation_Landscape
} OF_ScreenOrientation;

typedef enum _OF_SelectColor
{
    OF_SelectColor_Red,
    OF_SelectColor_Yellow,
    OF_SelectColor_Green,
    OF_SelectColor_Cyan,
    OF_SelectColor_Blue,
    OF_SelectColor_Magenta,
    OF_SelectColor_White,
    OF_SelectColor_Middle,
    OF_SelectColor_Black
} OF_SelectColor;

typedef enum _OF_DeviceLevel
{
    OF_DeviceLevel_0 = 0,                   /* right now it means the device is high level. */
    OF_DeviceLevel_1,                       /* right now it means the device is normal level. */
    OF_DeviceLevel_2                        /* right now it means the device is low level. */
} OF_DeviceLevel;

typedef enum _OF_ConfigKey
{
    OF_ConfigKey_RenderType = 0,            /* OF_RendererType, Default is OF_RendererType_GLES_2_0 */
    OF_ConfigKey_GlobalResDir,              /* OFPath, Global global search path. */
    OF_ConfigKey_IsSaveJsonPretty,          /* OFBool, Whether to save the json is pretty. Default is OF_FALSE. */
    OF_ConfigKey_IsMirror,                  /* OFBool, Whether the input texture is mirrored. Default is OF_TRUE. */
    OF_ConfigKey_ScreenWidth,               /* OFInt32, Screen width of device, some filter may use. Default is 720. */
    OF_ConfigKey_ScreenHeight,              /* OFInt32, Screen height of device, some filter may use. Default is 1280. */
    OF_ConfigKey_ScreenOrientation,         /* OF_ScreenOrientation, Screen orientation. Default is OF_ScreenOrientation_Portrait. */
    OF_ConfigKey_IsSaveEffectLatestVersion, /* OFBool, Whether to use the latest version to save the effect file when use OF_SaveEffectXXX. Default is OF_TRUE. */
    OF_ConfigKey_Animation2DPlaySpeed,      /* float, Specifies the animation playback speed, default is 1.0f. */
    OF_ConfigKey_DeviceLevel,               /* OF_DeviceLevel, Specifies the device performance level, default is OF_DeviceLevel_1. */
    OF_ConfigKey_IsAsyncLoadIO,             /* OF_ConfigKey_IsAsyncLoadIO. */
    OF_ConfigKey_IsProfilerOn,              /* OF_ConfigKey_IsProfilerOn. */

} OF_ConfigKey;

typedef enum _OF_TextType
{
    OF_TextType_CharMap,
    OF_TextType_TrueType,
    OF_TextType_SystemFont
} OF_TextType;

typedef enum _OF_ParticleSystem
{
    OF_Particle_Invalid = 0,                /* invalid flag */
    OF_Particle_State,
    OF_Particle_Quota,
    OF_Emitter_EmissionRate,
    OF_Emitter_EmissionPlace,
    OF_Emitter_Angle,
    OF_Emitter_Radius,
    OF_Emitter_TimeToLive,
    OF_Emitter_Velocity,
    OF_Emitter_Scale,
    OF_Emitter_Type,
    OF_Emitter_Color,
    OF_Render_Texture,
    OF_Render_BlendMode,
    OF_Affector_Type,
    OF_Affector_Count,
    OF_Affector_Create,
    OF_Affector_Delete,
    OF_Affector_Value,
    OF_Affector_Color
} OF_ParticleSystem;

typedef enum _OF_EffectPlayMode
{
    OF_EffectPlayMode_Loop,
    OF_EffectPlayMode_OneTime
} OF_EffectPlayMode;

typedef enum _OF_AnimatorPlayMode
{
    OF_AnimatorPlayMode_Loop,
    OF_AnimatorPlayMode_AlternatingLoop,
    OF_AnimatorPlayMode_OneTime,
    OF_AnimatorPlayMode_OneTimeKeep,
    OF_AnimatorPlayMode_CustomLoops
} OF_AnimatorPlayMode;

typedef enum _OF_BasketballGameEvent
{
    OF_GameEvent_CreateBall,
    OF_GameEvent_GameOver,
    OF_GameEvent_ScoreChanged
} OF_BasketballGameEvent;

typedef enum _OF_FoodGameEvent
{
    OF_FoodGameEvent_GameOver,
    OF_FoodGameEvent_ScoreChanged
} OF_FoodGameEvent;

typedef enum _OF_PoseMatchGameEvent
{
    OF_PoseMatchGameEvent_PoseData,
    OF_PoseMatchGameEvent_PoseResult,
} OF_PoseMatchGameEvent;

typedef enum _OF_SegmentType
{
    OF_SegmentType_Portrait,
    OF_SegmentType_Hair
} OF_SegmentType;

typedef enum _OF_ActionTriggers
{
    OF_ActionTriggers_OpenMouth             = 0x00000001,
    OF_ActionTriggers_CloseMouth            = 0x00000002,
    OF_ActionTriggers_Portrait              = 0x00000004,
    OF_ActionTriggers_Landscape             = 0x00000008,
    OF_ActionTriggers_Duration              = 0x00000010,
    OF_ActionTriggers_EyeBlink              = 0x00000020,
    OF_ActionTriggers_HeadYaw               = 0x00000040,
    OF_ActionTriggers_HeadPitch             = 0x00000080,
    OF_ActionTriggers_BrowJump              = 0x00000100,
    OF_ActionTriggers_GestureV              = 0x00000200,
    OF_ActionTriggers_GestureThumbUp        = 0x00000400,
    OF_ActionTriggers_GestureOneHandHeart   = 0x00000800,
    OF_ActionTriggers_GestureLift           = 0x00001000,
    OF_ActionTriggers_GestureTwoHandHeart   = 0x00002000,
    OF_ActionTriggers_AudioBeat             = 0x00004000,
    OF_ActionTriggers_AudioLoudness         = 0x00008000,
    OF_ActionTriggers_AudioSpectrum         = 0x00010000,
    OF_ActionTriggers_GestureSpiderman      = 0x00020000,
    OF_ActionTriggers_Gesture_666           = 0x00040000,
    OF_ActionTriggers_GestureZuoYi          = 0x00080000,
    OF_ActionTriggers_GesturePalmOpen       = 0x00100000,
    OF_ActionTriggers_GesturePalmTogether   = 0x00200000,
    OF_ActionTriggers_GesturePalmOK         = 0x00400000,
    OF_ActionTriggers_GesturePistol         = 0x00800000,
    OF_ActionTriggers_GestureFingerIndex    = 0x01000000,
    OF_ActionTriggers_GestureFist           = 0x02000000,
    OF_ActionTriggers_GestureILoveYou       = 0x04000000,
    OF_ActionTriggers_All                   = 0xFFFFFFFF
} OF_ActionTriggers;

typedef enum _OF_RequiredFrameData
{
    OF_RequiredFrameData_None               = 0x00000000,
    OF_RequiredFrameData_FaceLandmarker     = 0x00000001,
    OF_RequiredFrameData_HeadPoseEstimate   = 0x00000002,
    OF_RequiredFrameData_FaceAction         = 0x00000004,
    OF_RequiredFrameData_Gesture            = 0x00000008,
    OF_RequiredFrameData_Body               = 0x00000010,
    OF_RequiredFrameData_BackgroundSegment  = 0x00000020,
    OF_RequiredFrameData_ArCamera           = 0x00000040,
    OF_RequiredFrameData_AudioBeat          = 0x00000080,
    OF_RequiredFrameData_Avatar             = 0x00000100,
    OF_RequiredFrameData_Body3D             = 0x00000200,
    OF_RequiredFrameData_MicVolume          = 0x00000400,
    OF_RequiredFrameData_BodyContour        = 0x00000800,
    OF_RequiredFrameData_HairSegment        = 0x00001000,
    OF_RequiredFrameData_ClothesSegment     = 0x00002000,
	OF_RequiredFrameData_FaceOccludeSegment = 0x00004000,
    OF_RequiredFrameData_Max                = 0x00004000,
    OF_RequiredFrameData_All                = 0xFFFFFFFF
} OF_RequiredFrameData;

typedef enum _OF_ResType
{
    OF_ResType_Image                        = 0x00000001,
    OF_ResType_3DMesh                       = 0x00000002,
    OF_ResType_Particle                     = 0x00000004,
    OF_ResType_SVGA                         = 0x00000008,
    OF_ResType_TTF                          = 0x00000010,
    OF_ResType_LuaScript                    = 0x00000020,
    OF_ResType_Music                        = 0x00000040,
    OF_ResType_3DLUT                        = 0x00000080,
    OF_ResType_VS                           = 0x00000100,
    OF_ResType_PS                           = 0x00000200,
    OF_ResType_WebM                         = 0x00000400,
    OF_ResType_Animation                    = 0x00000800,
    OF_ResType_Video                        = 0x00001000,
    OF_ResType_Effect                       = 0x00002000,
} OF_ResType;

typedef enum _OF_ParamType
{
    OF_ParamType_Float  = 0,
    OF_ParamType_Int    = 1,
    OF_ParamType_Bool   = 2,
    OF_ParamType_Enum   = 3,
    OF_ParamType_Color  = 4,
    OF_ParamType_Res    = 5,
    OF_ParamType_ResArr = 6, // not finish.
    OF_ParamType_String = 7,
    OF_ParamType_MakeupRes = 8,
	OF_ParamType_Path = 9,          // path,
    OF_ParamType_Binary = 10,       // binary
    OF_ParamType_FloatArray = 11,
} OF_ParamType;

/* makeup extension point type*/
typedef enum _OF_Makeups
{
	OF_Makeups_Other = 0, OF_Eye_shadow = 1, OF_Eyelash = 2, OF_Lips = 3, OF_Eye_sparkle = 4, OF_Brow = 5, OF_Blusher = 6, OF_Facialfeatures = 7, OF_Pupil = 8,
	OF_Eyelash1 = 100, OF_Eye_shadow1 = 101, OF_Lips1 = 102
} OF_Makeups;

/************************************************************************/
/* Some base interface definition.                                      */
/************************************************************************/
typedef struct _OF_Paramf
{
    char name[ 64 ];
    float minVal;
    float maxVal;
    float val;
    float defVal;
    OFInt32 nCurveIndex;
} OF_Paramf;

typedef struct _OF_ParamFloatArray {
    char name[64];
    float val[32];
    float defVal[32];
    OFInt32 count;
} OF_ParamFloatArray;

typedef struct _OF_EffectParamfData
{
    OFUInt32 filterIndex;
    OFUInt32 paramfIndex;
} OF_EffectParamfData;

typedef struct _OF_Parami
{
    char name[ 64 ];
    OFInt32 minVal;
    OFInt32 maxVal;
    OFInt32 val;
    OFInt32 defVal;
} OF_Parami;

typedef struct _OF_ParamBool
{
    char name[ 64 ];
    OFBool val;
    OFBool defVal;
} OF_ParamBool;
    
typedef struct _OF_ParamString 
{
    char name[ 64 ];
    OFText val;
    OFText defVal;
} OF_ParamString;

typedef struct _OF_ParamPath
{
	char name[64];
	OFText val;
	OFText defVal;
} OF_ParamPath;

typedef struct _OF_ParamBinary
{
    char name[64];
    OFName resName;
    OFUInt32 width;
    OFUInt32 height;
    OFUInt8* data;
    OFUInt32 dataLength;
    OFUInt32 format;
} OF_ParamBinary;

typedef struct _OF_ParamEnum
{
    char name[ 64 ];
    OFUInt32 val;
    OFUInt32 defVal;
    OFUInt32 enumCount;
    OFName enumList[ 40 ];
} OF_ParamEnum;

typedef struct _OF_ParamRes
{
    char name[ 64 ];
    OFUInt32 resType;
    OFName resName;
} OF_ParamRes;

typedef struct _OF_ParamMakeupRes
{
    char name[64];
    OFUInt32 resType;
    OFName resName;
    OFUInt32 textureCoordsCount;
    OFUInt32 indicesCount;
    OFUInt16 indices[400 * 3];
    float textureCoords[400 * 2]; 
} OF_ParamMakeupRes;
#define OF_MAX_PARAM_RES_ARR_SIZE 100
typedef struct _OF_ParamResArr
{
    char name[ 64 ];
    OFUInt32 resType;
    OFUInt32 resCount;
    OFName resNames[OF_MAX_PARAM_RES_ARR_SIZE];
} OF_ParamResArr;

typedef struct _OF_ParamColor
{
    char name[64];
    float val[4];
    float defVal[4];
    OFInt32 nCurveIndex;
} OF_ParamColor;
typedef struct _OF_Param
{
    OFName name;
    OF_ParamType type;
    union
    {
        OF_Paramf* paramf;
        OF_Parami* parami;
        OF_ParamBool* paramBool;
        OF_ParamEnum* paramEnum;
        OF_ParamRes* paramRes;
        OF_ParamResArr* paramResArr;
        OF_ParamColor* paramColor;
        OF_ParamString* paramString;
        OF_ParamMakeupRes* paramMakeupRes;
		OF_ParamPath* paramPath;
        OF_ParamBinary* paramBinary;
        OF_ParamFloatArray* paramFloatArray;
    } data;
} OF_Param;

typedef struct _OF_EffectTrackData
{
    OFUInt64 timestamp;
    float x;
    float y;
} OF_EffectTrackData;

typedef struct _OF_InputMediaData
{
    OFName type;                       /* one of "camera", "video" or "image" */
    OFPath url;                        /* if type = "camera", url is unused */
} OF_InputMediaData;

typedef struct _OF_Locus_UserImage
{
    bool isUsed;                       /* is using user image for locus face detect*/
    OFPath userImagePath;              /*user image path*/
}OF_Locus_UserImage;

#define OF_MAX_EFFECT_INPUT_SIZE    5
#define OF_MAX_EFFECT_FILTERS_SIZE  256
#define OF_MAX_EFFECT_ANIMATOR_SIZE 30
#define OF_MAX_EFFECT_SCENE_SIZE    30
#define OF_MAX_EFFECT_PARAMS_SIZE   10
typedef struct _OF_EffectInfo
{
    OFUInt32 version;
    OFPath resPath;
    OFName luaScriptName;
    OFName audioName;
    OFUInt64 duration;
    OF_EffectPlayMode playMode;
    OFUInt64 timestamp;
    OFBool isFadeout;
    float fadeoutStart;
    OFUInt64 trackDataCacheTime;
    OFUInt32 inputCount;
    OF_InputMediaData inputList[ OF_MAX_EFFECT_INPUT_SIZE ];
    OFUInt32 filterCount;
    OFHandle filterList[ OF_MAX_EFFECT_FILTERS_SIZE ];
    OFUInt64 filterTimeRange[ OF_MAX_EFFECT_FILTERS_SIZE ][2];
    OFUInt32 paramfCount;
    OF_EffectParamfData paramfList[ OF_MAX_EFFECT_PARAMS_SIZE ];
    OFUInt32 animatorCount;
    OFHandle animatorList[ OF_MAX_EFFECT_ANIMATOR_SIZE ];
    OFUInt32 sceneCount;
    OFHandle sceneList[ OF_MAX_EFFECT_SCENE_SIZE ];
    OF_Locus_UserImage userimage;
} OF_EffectInfo;

typedef struct _OF_ProfilerInfo
{
    float internalMemory;
    float graphicsMemory;
} OF_ProfilerInfo;

/************************************************************************/
/* Filters extra data definition.                                       */
/************************************************************************/
/* KFilterMirror */
typedef struct _OF_MirrorFilterExtraData
{
    OFBool horizontally;
    OFBool vertically;
} OF_MirrorFilterExtraData;

/* KFilterCurve */
typedef struct _OF_CurveFilterExtraData
{
    OFUInt8 redLUT[ 256 ];
    OFUInt8 greenLUT[ 256 ];
    OFUInt8 blueLUT[ 256 ];
    OFUInt8 rgbLUT[ 256 ];
    OFInt32 redKeyPoints[ 512 ];
    OFInt32 greenKeyPoints[ 512 ];
    OFInt32 blueKeyPoints[ 512 ];
    OFInt32 rgbKeyPoints[ 512 ];
    OFUInt32 redKeyPointsCount;
    OFUInt32 greenKeyPointsCount;
    OFUInt32 blueKeyPointsCount;
    OFUInt32 rgbKeyPointsCount;
} OF_CurveFilterExtraData;

/* KFilterBlendTable */
typedef struct _OF_BlendFilterExtraData
{
    OFPath blendImagePath;
    OF_BlendMode blendMode;
    OFBool isMirror;
} OF_BlendFilterExtraData;

/* KFilterPaint */
typedef struct _OF_PaintFilterExtraData
{
    OFPath penTexturePath;
    OFUInt32 penRadius;
    float opacity;
    float penColor[ 4 ];
    OFBool clearMode;
} OF_PaintFilterExtraData;

/* KFilterLookUpTable */
typedef struct _OF_LookUpTableFilterExtraData
{
    OFPath lutPath;
} OF_LookUpTableFilterExtraData;

typedef struct _OF_SoftGlowFilterExtraData
{
    OFPath lutPath;
    OF_SoftGlowBlendMode blendMode;
} OF_SoftGlowFilterExtraData;

typedef struct _OF_BrowseFilterExtraData
{
    OF_BrowseMode browseMode;
} OF_BrowseFilterExtraData;

/* KFilterLookUpTable3D */
typedef struct _OF_LookUpTable3DFilterExtraData
{
    OFPath lutPath;
} OF_LookUpTable3DFilterExtraData;

typedef struct _OF_EdgeMaskLookUpTableFilterExtraData
{
    OFPath lutPath;
    OF_EdgeDetectMode edgeDetectMode;
} OF_EdgeMaskLookUpTableFilterExtraData;

/* KFilterMaskLookUpTable */
typedef struct _OF_MaskLookUpTableFilterExtraData
{
    OFPath lutPath;
    OFPath maskPath;
} OF_MaskLookUpTableFilterExtraData;

/* KFilterBeautyMakeup0 */
typedef struct _OF_BeautyMakeupFilter0ExtraData
{
    OFPath lutPath;                         /* Full png image path for lut texture. */
    OFPath maskPath;                        /* Full png image path for mask texture. */
    float textureCoords[ (106 + 14) * 2 ];  /* [t0x, t0y, t1x, t1y, t2x, t2y, ...]. */
    OFUInt32 textureCoordsCount;
    OFUInt16 indices[ 600 ];                /* [i0x, i0y, i0z, i1x, i1y, i1z, ...]. */
    OFUInt32 indicesCount;
} OF_BeautyMakeupFilter0ExtraData;

/* Face's Extension point.                                              */
#define OF_EXT_VER_NUM 72

/* KFilterBeautyMakeup */
typedef struct _OF_BeautyMakeupFilterExtraData
{
    OFPath lutPath;
	OFPath toothLutPath;
    OFPath maskPath;
    float textureCoords[(278 + OF_EXT_VER_NUM) * 2];    /* [t0x, t0y, t1x, t1y, t2x, t2y, ...]. */
    OFUInt32 textureCoordsCount;
    OFUInt16 indices[400 * 3];                           /* [i0x, i0y, i0z, i1x, i1y, i1z, ...]. */
    OFUInt32 indicesCount;
	OF_Makeups makeups;
    OF_FaceLandmarkMode landmarkMode;
} OF_BeautyMakeupFilterExtraData;

/* KFilterSelectiveColor */
typedef struct _OF_SelectiveColorAdjust
{
    float cyan;                             /* [-1, 1] */
    float magenta;                          /* [-1, 1] */
    float yellow;                           /* [-1, 1] */
    float black;                            /* [-1, 1] */
} OF_SelectiveColorAdjust;

typedef struct _OF_SelectiveColorFilterExtraData
{
    OF_SelectiveColorAdjust colors[ OF_SelectColor_Black + 1 ];
    OFBool isAbsolute;
} OF_SelectiveColorFilterExtraData;

/* KFilterFace2dMask */
typedef struct _OF_Face2dMaskExtraData
{
    OFPath faceMaskPath;                    /* Full png image path for mask texture. */
    float textureCoords[106 * 2];           /* [t0x, t0y, t1x, t1y, t2x, t2y, ...]. */
    OFUInt32 textureCoordsCount;
    OFUInt16 indices[600];                  /* [i0x, i0y, i0z, i1x, i1y, i1z, ...]. */
    OFUInt32 indicesCount;
    OF_BlendMode blendMode;
    OF_FaceLandmarkMode faceLandmarkMode;
} OF_Face2dMaskFilterExtraData;

/* KFilterFacialMask */
typedef struct _OF_FacialMaskFilterExtraData
{
    OFPath maskPath;                                            /* Full png image path for mask texture. */
    OFPath weightMapPath;
    float textureCoords[ (278 + OF_EXT_VER_NUM) * 2 ];         /* [t0x, t0y, t1x, t1y, t2x, t2y, ...].  */
    OFUInt32 textureCoordsCount;
    OFUInt16 indices[400 * 3];                                  /* [i0x, i0y, i0z, i1x, i1y, i1z, ...].  */
    OFUInt32 indicesCount;
    float weightMapTextureCoords[(278 + OF_EXT_VER_NUM) * 2];  /* [t0x, t0y, t1x, t1y, t2x, t2y, ...].  */
    OFUInt32 weightMapTextureCoordsCount;
    OFUInt16 weightMapIndices[400 * 3];                         /* [i0x, i0y, i0z, i1x, i1y, i1z, ...].  */
    OFUInt32 weightMapIndicesCount;
    OF_BlendMode blendMode;
    OFBool isSeamlessBlend;
	OF_Makeups makeups;
    OF_FaceLandmarkMode landmarkMode;
} OF_FacialMaskFilterExtraData;

/* KFilterFacialMask */
typedef struct _OF_FacialStickerFilterExtraData
{
    OFPath maskPath;                                /* Full png image path for mask texture. */
    OFPath weightMapPath;
    float textureCoords[(106 + 5 + 33 + 5) * 2];    /* [t0x, t0y, t1x, t1y, t2x, t2y, ...]. */
    OFUInt32 textureCoordsCount;
    OFUInt16 indices[800];                          /* [i0x, i0y, i0z, i1x, i1y, i1z, ...]. */
    OFUInt32 indicesCount;
    float weightMapTextureCoords[(106 + 5) * 2];    /* [t0x, t0y, t1x, t1y, t2x, t2y, ...]. */
    OFUInt32 weightMapTextureCoordsCount;
    OFUInt16 weightMapIndices[600];                 /* [i0x, i0y, i0z, i1x, i1y, i1z, ...]. */
    OFUInt32 weightMapIndicesCount;
    OF_BlendMode blendMode;
    OFBool isBlendMode;
    OF_FaceLandmarkMode landmarkMode;
    OFInt32 CenterpointIndex;
} OF_FacialStickerFilterExtraData;

/* KFilterFacialMaskAnimation */
typedef struct _OF_FacialMaskAnimationFilterExtraData
{
    OFPath maskPngArr[80];                                       /* Full png image path for mask texture. */
    OFUInt32 maskPngArrCount;
    OFPath weightMapPath;
    OFUInt32 timeInterval;
    OFUInt32 triggers;
    float openMouthThreshold;
    float textureCoords[(278 + OF_EXT_VER_NUM) * 2];            /* [t0x, t0y, t1x, t1y, t2x, t2y, ...]. */
    OFUInt32 textureCoordsCount;
    OFUInt16 indices[400*3];                                     /* [i0x, i0y, i0z, i1x, i1y, i1z, ...]. */
    OFUInt32 indicesCount;
    float weightMapTextureCoords[(278 + OF_EXT_VER_NUM) * 2];   /* [t0x, t0y, t1x, t1y, t2x, t2y, ...]. */
    OFUInt32 weightMapTextureCoordsCount;
    OFUInt16 weightMapIndices[400 * 3];                          /* [i0x, i0y, i0z, i1x, i1y, i1z, ...]. */
    OFUInt32 weightMapIndicesCount;
    OF_BlendMode blendMode;
    OFBool isSeamlessBlend;
	OF_Makeups makeups;
    OF_FaceLandmarkMode landmarkMode;
} OF_FacialMaskAnimationFilterExtraData;

/* KFilterFaceSegment */
typedef struct _OF_FaceSegmentFilterExtraData
{
    OFPath weightMapPath;
} OF_FaceSegmentFilterExtraData;

/* KFilterFace2dAnimation */
typedef struct _OF_2dAnimationData
{
    OFPath* pngPathArr;                     /* Memory is managed by the caller. */
    OFUInt32 pngPathArrCount;
    OFInt32 timeInterval;
    OFInt32 spaceTime;
    OF_BlendMode blendMode;
    float opacity;                          /* [0.0, 1.0]. */
    float rectTextureCoords[ 8 ];
    OFInt16 alignFaceIndices[ 3 ];
    float alignFaceTextureCoords[ 6 ];

    /* Used for fixed stickers, if alignFaceIndices[ 0, 1, 2 ] == -1 */
    OFInt32 imageWidth;
    OFInt32 imageHeight;
    float portraitOffset[ 2 ];
    float landscapeOffset[ 2 ];
} OF_2dAnimationData;

typedef struct _OF_Face2dAnimationFilterExtraData
{
    OF_2dAnimationData animationArr[ 10 ];
    OFUInt32 animationArrCount;
    OFUInt32 triggers;
    float openMouthThreshold;
} OF_Face2dAnimationFilterExtraData;

/* KFilterBlendAnimation */
typedef struct _OF_BlendAnimationFilterExtraData
{
    OFPath pngPathArr[ 80 ];
    OFUInt32 pngPathArrCount;
    OF_BlendMode blendMode;
    float opacity;
    OFUInt32 timeInterval;
    OFBool isMirror;
    OFName svgaFile;
    OFName webmFile;
    OFBool svgaUseCustomTimeInterval;
} OF_BlendAnimationFilterExtraData;

/*KFilterMattingBlend*/
typedef struct _OF_MattingBlendFilterExtraData
{
    OFPath pngPathArr[ 80 ];
    OFUInt32 pngPathArrCount;
    float opacity;
    OFUInt32 timeInterval;
    OFBool isMirror;
} OF_MattingBlendFilterExtraData;

/* KFilterFixedStickersAnimation */
typedef struct _OF_FixedStickersAnimationFilterExtraData
{
    OFPath pngPathArr[ 80 ];
    OFUInt32 pngPathArrCount;
    OFInt32 imageWidth;
    OFInt32 imageHeight;
    float portraitOffset[ 2 ];
    float landscapeOffset[ 2 ];
    OF_BlendMode blendMode;
    float opacity;
    OFUInt32 timeInterval;
    OFUInt32 triggers;
    OFInt32 fixedFrameIdx;
    OFName svgaFile;
    OFName webmFile;
    OFBool svgaUseCustomTimeInterval;
} OF_FixedStickersAnimationFilterExtraData;

/* KFilterDynStickersAnimation */
typedef struct _OF_DynStickersAnimationFilterExtraData
{
    OFPath pngPathArr[ 80 ];
    OFUInt32 pngPathArrCount;
    OFInt32 imageWidth;
    OFInt32 imageHeight;
    OFInt32 designWidth;
    OFInt32 designHeight;
    OF_BlendMode blendMode;
    float opacity;
    OFUInt32 timeInterval;
    OFInt32 fixedFrameIdx;
    OFBool clampLastFrame;
    OFBool customUV;
    float uv[ 8 ];
    OFBool clip2x3;
    OFName svgaFile;
    OFBool svgaUseCustomTimeInterval;
} OF_DynStickersAnimationFilterExtraData;

/* KFilterLookUpTableAnimation */
typedef struct _OF_LookUpTableAnimationFilterExtraData
{
    OFPath lutPathArr[ 50 ];
    OFUInt32 lutPathArrCount;
    OFUInt32 period;
    OFBool isSmooth;
} OF_LookUpTableAnimationFilterExtraData;

/* KFilterEdgeLookUpTableAnimation */
typedef struct _OF_EdgeLookUpTableAnimationFilterExtraData
{
    OFPath lutPathArr[ 50 ];
    OFUInt32 lutPathArrCount;
    OFUInt32 period;
    OF_EdgeDetectMode edgeDetectMode;
} OF_EdgeLookUpTableAnimationFilterExtraData;

/* KFilterFace3dPlaneAnimation */
typedef struct _OF_Face3dPlaneAnimationFilterExtraData
{
    OFPath pngPathArr[ 80 ];
    OFUInt32 pngPathArrCount;
    OF_BlendMode blendMode;
    float opacity;
    OFUInt32 timeInterval;
    OFUInt32 triggers;
    float openMouthThreshold;
    OFInt32 fixedFrameIdx;
    OFName svgaFile;
    OFName webmFile;
    float pngHeightWidthRatio;
    OFBool svgaUseCustomTimeInterval;
} OF_Face3dPlaneAnimationFilterExtraData;

/* KFilterTrackPlaneAnimation */
typedef struct _OF_TrackPlaneAnimationFilterExtraData
{
    OFPath pngPathArr[ 80 ];
    OFUInt32 pngPathArrCount;
    OF_AnimatorPlayMode animatorPlayMode;
    OF_BlendMode blendMode;
    float opacity;
    OFUInt32 timeInterval;
    OFBool useNearestPointFilter;
    OFBool colorEnable;
    OFName svgaFile;
    OFName webmFile;
    OFUInt32 framePartition[3];
    OFName musicFiles[3];
    OF_LocationMode locationMode;
    OFInt32 designWidth;
    OFInt32 designHeight;
    float   pngHeightWidthRatio;
    bool useAlpha;
} OF_TrackPlaneAnimationFilterExtraData;

/* KFilterTrackText */
typedef struct _OF_TrackTextFilterExtraData
{
    OFUInt32 period;
    OFUInt8 fontIdx;
    OFName fontName;
    OFPath fontDir;
    OFUInt8 anchor;
    OFUInt8 size;
    OFUInt8 spacing;
    OFUInt8 color[ 4 ];
    OFUInt8 effectIdx;
    OFUInt8 direction;
    OFBool autoScale;
    OFBool bold;
    OFBool shadow;
    OFUInt16 maxLetterCnt;
    OF_AnimatorPlayMode animatorPlayMode;
    OFText text;
    float stickerWidthRatio;
    float stickerMultiple;
    OFUInt16 stickerWidth;
    OFUInt16 stickerHeight;
    OF_LocationMode locationMode;
    OF_BlendMode blendMode;
    bool useAlpha;
    OFInt32 designWidth;
    OFInt32 designHeight;
} OF_TrackTextFilterExtraData;

/* KFilterFace3dMesh */
typedef struct _OF_Face3dMeshFilterExtraData
{
    OFPath meshPath;
    OFPath texturePath;
    OFUInt32 timeInterval;
    OFUInt32 triggers;
    float openMouthThreshold;
    OFInt32 animationClipIndex;
    OFInt32 animationPlayMode;
} OF_Face3dMeshFilterExtraData;

/* KFilterTrackParticleSystem */
typedef struct _OF_ParticleSystemFilterExtraData
{
    OFPath modelPath;
    OFBool colorEnable;
} OF_ParticleSystemFilterExtraData;

/* KFilterGameFood */
typedef struct _OF_GameFoodFilterExtraData
{
    OFInt32 gameMode;
    OFInt32 speedMode;
} OF_GameFoodFilterExtraData;

/* KFilterFaceDeform */
typedef struct _OF_FaceDeformFilterExtraData
{
    float deformVec2[ 106 * 2 ];
} OF_FaceDeformFilterExtraData;

/* KFilterPupilSwap */
typedef struct _OF_PupilSwapFilterExtraData
{
    OFPath leftPupilPngPath;
    OFPath rightPupilPngPath;
    OFPath maskPngPath;
    float maskTextureCoords[212];             /* [t0x, t0y, t1x, t1y, t2x, t2y, ...]. */
    //float maskTextureCoords[(44+4) * 2];      /* [t0x, t0y, t1x, t1y, t2x, t2y, ...]. */
    OFUInt32 maskTextureCoordsCount;
    OFUInt16 maskIndices[132];              /* [i0x, i0y, i0z, i1x, i1y, i1z, ...]. */
    OFUInt32 maskIndicesCount;
    OF_BlendMode blendMode;
    OF_FaceLandmarkMode faceLandmarkMode;
} OF_PupilSwapFilterExtraData;

/* KFilterFaceSwap */
typedef struct _OF_FaceSwapFilterExtraData
{
    OFPath facePngPath;
    float facePoints[278 * 2];
    OFUInt32 facePointsCoordsCount;
    OFPath maskPngPath;
    float masktextureCoords[278 * 2];
    OFUInt32 masktextureCoordsCount;
    OFUInt16 faceindices[400 * 3];
    OFUInt32 faceindicesCount;
    OFUInt16 maskindices[400 * 3];
    OFUInt32 maskindicesCount;
    OF_FaceLandmarkMode faceLandmarkMode;
} OF_FaceSwapFilterExtraData;

/*KFilterFaceLiquify*/
typedef struct _OF_FaceLiquifyFilterExtraData
{
    OFUInt8 typeFlag[106];
    float centerPoint[106 * 2];
    float radius[106];
    float weightMap[106];
    float moveDirect[106 * 2];
} OF_FaceLiquifyFilterExtraData;

typedef struct _OF_MultiFaceSwapFilterExtraData
{
    OFPath maskPngPath;
    float masktextureCoords[ 106 * 2 ];
    OFUInt32 masktextureCoordsCount;
    OFUInt16 maskindices[ 600 ];
    OFUInt32 maskindicesCount;
} OF_MultiFaceSwapFilterExtraData;

typedef struct _OF_NoiseTVFilterExtraData
{
    OFPath noisePngPath;
    OF_NoiseMode noiseMode;
} OF_NoiseTVFilterExtraData;

typedef struct _OF_ComponentMoveFilterExtraData
{
    OF_TimeVariableMode variableMode;
}OF_ComponentMoveFilterExtraData;

typedef struct _OF_SlicesFilterExtraData
{
    OFPath slicesPngPath;
    OF_SlicesMode slicesMode;
}OF_SlicesFilterExtraData;

typedef struct _OF_FrameStampFilterExtraData
{
    OFPath pngPathArr[ 80 ];
    OFUInt32 pngPathArrCount;
    OF_LocationMode locationMode;
    OFUInt32 timeInterval;
    OF_LayoutMode layoutMode;
    OFName svgaFile;
    OF_BlendMode blendMode;
    float opacity;
    OFBool svgaUseCustomTimeInterval;
}OF_FrameStampFilterExtraData;

/* KFilterFace3dPhysicsMesh */
typedef struct _OF_Face3dPhysicsMeshFilterExtraData
{
    OFPath pngPathArr[ 2 ];
    OFPath faceMeshPath;
    OFPath cookieMeshPath;
} OF_Face3dPhysicsMeshFilterExtraData;

/* KFilterParticle */
typedef struct _OF_ParticleFilterExtraData
{
    OF_ParticleSystem id;
    OFUInt32 affectorIndex;
    float data[ 20 ];
    OFUInt8 dataCount;
    OFPath texture;
    OFUInt32 triggers;
    float openMouthThreshold;
    OFBool colorEnable;
} OF_ParticleFilterExtraData;

/* KFilter3DAR */
typedef struct _OF_3DARFilterExtraData
{
    OFInt32 flag;
    OFPath sceneName;
    OFPath resDir;
} OF_3DARFilterExtraData;

/* KFilterPhysics */
typedef void(*BASKETBALL_FILTER_CALLBACK)(OFUInt32, OFUInt32, void*);
typedef struct _OF_BasketballFilterExtraData
{
    OFPath ballTexture;
    OFPath ballMeshPath;
    OFPath ballParticle;
    OFPath basketMeshPath;
    OFBool clip2x3;
    OFUInt32 action;
    void* userData;
    BASKETBALL_FILTER_CALLBACK func;
} OF_BasketballFilterExtraData;

/* KFilterFoodPhysics */
typedef void(*FOOD_FILTER_CALLBACK)(OFUInt32, OFUInt32, void*);
typedef struct _OF_FoodPhysicsFilterExtraData
{
    OFUInt32 action;
} OF_FoodPhysicsFilteExtraData;

/* KFilterText2dCharMap */
typedef struct _OF_Text2dFilterExtraData
{
    OF_TextType textType;
    OFPath fontFile;
    OFPath charmap;
    OFUInt32 charWidth;
    OFUInt32 charHeight;
    OFUInt8  startChar;
    OFUInt8 anchor;
    OFPath text;
    OFUInt32 designWidth;
    OFUInt32 designHeight;
} OF_Text2dFilterExtraData;

/* KFilterFixedStickersSVGA */
typedef struct _OF_FixedStickersSVGAFilterExtraData
{
    OFInt32 fixedFrames;
    OFInt32 fixedFrameIdx;
    OFInt32 imageWidth;
    OFInt32 imageHeight;
    float portraitOffset[ 2 ];
    float landscapeOffset[ 2 ];
    OF_BlendMode blendMode;
    OF_AnimatorPlayMode animatorPlayMode;
    float opacity;
    OFUInt32 timeInterval;
    OFUInt32 triggers;
    OFName svgaFile;
} OF_FixedStickersSVGAFilterExtraData;

typedef struct _OF_Svga2FilterExtraData {
    OFName svgaFile;
    /*OFInt32 imageWidth;
    OFInt32 imageHeight;
    float offset[2];*/
    OF_BlendMode blendMode;
    float opacity;
    OF_AnimatorPlayMode animatorPlayMode;
    OFInt32 loops;
    OFBool bIsVerticalFlip;
    OFBool bIsHorizontalFlip;
} OF_Svga2FilterExtraData;

/* KFilterCustomLua */
typedef struct _OF_CustomLuaFilterExtraData
{
    OFName luaScriptName;
    OFPath filterDir;
} OF_CustomLuaFilterExtraData;

/************************************************************************/
/* Animators extra data definition.                                     */
/************************************************************************/
/* KAnimatorFilterFloatParam */
typedef struct _OF_FilterFloatParamAnimatorExtraData
{
    OFUUID filter;
    OFUInt64 duration;
    OF_AnimatorPlayMode playMode;
    char paramName[ 64 ];
    OFUInt32 controlValueCount;
    float controlValueCurve[ 100 ];
} OF_FilterFloatParamAnimatorExtraData;

/* KAnimatorViewportsSceneParam */
typedef struct _OF_ViewportsSceneParamAnimatorExtraData
{
    OFUUID scene;
    OFUInt32 viewport;
    OFUInt64 duration;
    OF_AnimatorPlayMode playMode;
    char paramName[ 64 ];
    OFUInt32 controlValueCount;
    float controlValueCurve[ 100 ];
} OF_ViewportsSceneParamAnimatorExtraData;

/* KAnimatorAudioParam */
typedef struct _OF_AudioParamAnimatorExtraData
{
    OFUUID filter;
    char paramName[64];
    OFUInt32 controlValueCount;
    float controlValueCurve[100];
    OFBool smoothLoudness;
    OFBool enableWithoutAudio;
    OFUInt64 duration;
    OFBool useSpectrum;
    OFUInt16 beginSpectrumIndex;
    OFUInt16 endSpectrumIndex;
} OF_AudioParamAnimatorExtraData;

/************************************************************************/
/* Scenes extra data definition.                                        */
/************************************************************************/
#define OF_MAX_VIEWPORTS_SIZE  30
#define OF_MAX_FILTERS_SIZE    10
#define OF_MAX_FACESCENE_SIZE  5
#define OF_MAX_CLONE_SIZE 5
/* KSceneViewports */
typedef struct _OF_ViewportsData
{
    float x;
    float y;
    float width;
    float height;
    float scale;
    float offsetX;
    float offsetY;
    OFUInt32 filterCount;
    OFUUID filterList[ OF_MAX_FILTERS_SIZE ];
} OF_ViewportsData;

typedef struct _OF_ViewportsSceneExtraData
{
    OFUInt64 duration;
    OFUInt32 triggers;
    OFUInt16 beginSpectrumIndex;
    OFUInt16 endSpectrumIndex;
    OFUInt32 viewportsCount;
    OF_ViewportsData viewports[ OF_MAX_VIEWPORTS_SIZE ];
    OFUInt32 filterCount;
    OFUUID filterList[ OF_MAX_FILTERS_SIZE ];
} OF_ViewportsSceneExtraData;

/* KSceneSegment */
typedef struct _OF_SegmentSceneExtraData
{
    OFUInt64 duration;
    OFUInt32 triggers;
    OF_SegmentType segmentType;
    OFUInt32 backgroundFilterCount;
    OFUUID backgroundFilterList[ OF_MAX_FILTERS_SIZE ];
    OFUInt32 foregroundFilterCount;
    OFUUID foregroundFilterList[ OF_MAX_FILTERS_SIZE ];
    OFUInt32 postFilterCount;
    OFUUID postFilterList[ OF_MAX_FILTERS_SIZE ];
} OF_SegmentSceneExtraData;

/* KSceneClone */
typedef struct _OF_CloneSceneExtraData {
    OFUInt64 duration;
    OFUInt32 triggers;
    OF_SegmentType segmentType;
    OFUInt32 backgroundFilterCount;
    OFUUID backgroundFilterList[OF_MAX_FILTERS_SIZE];
    OFUInt32 foregroundFilterCount;
    OFUUID foregroundFilterList[OF_MAX_FILTERS_SIZE];
    OFUInt32 postFilterCount;
    OFUUID postFilterList[OF_MAX_FILTERS_SIZE];
    OFUInt32 cloneCount;
    OFUInt32 cloneFilterCountList[OF_MAX_CLONE_SIZE];
    OFUUID cloneFilterList[OF_MAX_CLONE_SIZE * OF_MAX_FILTERS_SIZE];
    float cloneTransformParams[OF_MAX_CLONE_SIZE * 4];
} OF_CloneSceneExtraData;

/* KSceneFaceSegment */
typedef struct _OF_FaceSegmentSceneExtraData {
    OFUInt64 duration;
    OFUInt32 triggers;
    OF_SegmentType segmentType;
    OFUInt32 backgroundFilterCount;
    OFUUID backgroundFilterList[OF_MAX_FILTERS_SIZE];
    OFUInt32 foregroundFilterCount;
    OFUUID foregroundFilterList[OF_MAX_FILTERS_SIZE];
    OFUInt32 postFilterCount;
    OFUUID postFilterList[OF_MAX_FILTERS_SIZE];
    OFUInt32 cloneCount;
    OFUInt32 cloneFilterCountList[OF_MAX_CLONE_SIZE];
    OFUUID cloneFilterList[OF_MAX_CLONE_SIZE * OF_MAX_FILTERS_SIZE];
    float cloneTransformParams[OF_MAX_CLONE_SIZE * 4];
    float maskTransformParams[5];
    OFPath weightMapPath;
    bool isSingalFaceSegment;
} OF_FaceSegmentSceneExtraData;

/* KSceneMultiFace */
typedef struct _OF_SingleFaceData
{
    OFUInt32 filterCount;
    OFUUID filterList[ OF_MAX_FILTERS_SIZE ];
} OF_SingleFaceData;

typedef struct _OF_MultiFaceSceneExtraData
{
    OFUInt64 duration;
    OFUInt32 triggers;
    OF_SingleFaceData faceDataArr[ OF_MAX_FACESCENE_SIZE ];
    OFUInt32 faceDataCount;
} OF_MultiFaceSceneExtraData;

typedef struct _OF_GestureSceneExtraData
{
    float boxscore;
    float nmsthresthold;
    OFUInt32 pngPathCount;
    OFPath pngPathArr[ OF_GestureType_Size ];
} OF_GestureSceneExtraData;

typedef struct _OF_GestureTrackTypeData
{
    OFUInt32 filterCount;
    OFUUID filterList[ OF_MAX_FILTERS_SIZE ];
} OF_GestureTrackTypeData;

typedef struct _OF_GestureTrackSceneExtraData
{
    OFUInt64 duration;
    OFUInt32 triggers;
    OF_GestureTrackTypeData gestureTrackTypeDataArr[ OF_GestureType_Retina_end];
    OFBool isAllType;
} OF_GestureTrackSceneExtraData;

/* KSceneStamp */
typedef struct _OF_StampSceneExtraData
{
    OFPath pngArr[80];
    OFUInt32 pngArrCount;
    OFPath svgaFile;
    OFPath webmFile;
    OF_AnimatorPlayMode playMode;
    OFUInt32 timeInterval;
    float xOffset;
    float yOffset;
    float rotate;
    float scale;
    OF_BlendMode blendMode;
    float opacity;
    OF_LocationMode locationMode;
    OFUInt64 duration;
    OFUInt32 triggers;
    OFUInt32 stampFilterCount;
    OFUUID stampFilterList[OF_MAX_FILTERS_SIZE];
    OFUInt32 postFilterCount;
    OFUUID postFilterList[OF_MAX_FILTERS_SIZE];
    OFUInt32 preFilterCount;
    OFUUID preFilterList[OF_MAX_FILTERS_SIZE];
    OFBool isTrackable;
} OF_StampSceneExtraData;

/************************************************************************/
/* Games extra data definition.                                         */
/************************************************************************/
/* Basketball Game */
typedef struct _OF_BasketballGameEventData
{
    OFUInt32 ballNo;
    OFUInt32 totalCombo;
    OFUInt32 finalScore;
} OF_BasketballGameEventData;

typedef void(*BASKETBALL_GAME_CALLBACK)(OF_BasketballGameEvent, OF_BasketballGameEventData);
typedef struct _OF_BasketballGameData
{
    OFUInt32 gameMode;
    OFUInt32 topScore;
    BASKETBALL_GAME_CALLBACK gameCallbackFunc;
} OF_BasketballGameData;

/* Food Game */
typedef struct _OF_FoodGameEventData
{
    OFUInt32 goodCnt;
    OFUInt32 perfectCnt;
    OFUInt32 finalScore;
} OF_FoodGameEventData;

typedef void(*FOOD_GAME_CALLBACK)(OF_FoodGameEvent, OF_FoodGameEventData);
typedef struct _OF_FoodGameData
{
    OFUInt32 gameMode;
    OFUInt32 topScore;
    OFInt32 faceIndex;
    OFInt32 speedMode;
    OFInt32 isLowDevice;
    OFInt32 isShowLogo;
    FOOD_GAME_CALLBACK gameCallbackFunc;
} OF_FoodGameData;

/* Pose Match Game */
typedef struct _OF_PoseMatchGameEventData
{
    OFInt32 id;
    char poseData[1024 * 10]; // pose data in json string
    float poseSimilarRatio; // 0.0 ~ 1.0, < 0 error.

    // reserved params for custom.
    //float paramFloat1;
    //float paramFloat2;
    char paramStr1[128];
    char paramStr2[128];
} OF_PoseMatchGameEventData;

typedef void(*PoseMatch_GAME_CALLBACK)(OF_PoseMatchGameEvent, OF_PoseMatchGameEventData*);
typedef struct _OF_PoseMatchGameData
{
    PoseMatch_GAME_CALLBACK gameCallbackFunc;
} OF_PoseMatchGameData;

/************************************************************************/
/* Global exported API definition.                                      */
/************************************************************************/
OF_EFFECT_EXPORT_API OF_Result OF_CreateContext(
    OFHandle* contextID);                       /* [out] */

OF_EFFECT_EXPORT_API OF_Result OF_DestroyContext(
    OFHandle contextID);                        /* [in] */

OF_EFFECT_EXPORT_API OF_Result OF_SetConfig(
    OFHandle contextID,                         /* [in] */
    OF_ConfigKey key,                           /* [in] */
    void* value);                               /* [in] */

OF_EFFECT_EXPORT_API OF_Result OF_GetConfig(
    OFHandle contextID,                         /* [in] */
    OF_ConfigKey key,                           /* [in] */
    void* value);                               /* [out] */

OF_EFFECT_EXPORT_API OF_Result OF_CreateFilter(
    OFHandle contextID,                         /* [in] */
    const char* filterType,                     /* [in] */
    OFHandle* filterID);                        /* [out] */

OF_EFFECT_EXPORT_API OF_Result OF_DestroyFilter(
    OFHandle contextID,                         /* [in] */
    OFHandle filterID);                         /* [in] */

OF_EFFECT_EXPORT_API OF_Result OF_GetFilterParamCount(
    OFHandle contextID,                         /* [in] */
    OFHandle filterID,                          /* [in] */
    OFUInt32* paramCount);                      /* [out] */

/**
 * Query filter parameter name by index.
 * @param[in] contextID, context.
 * @param[in] filterID, filter to query.
 * @param[in] paramIndex, parameter index to query.
 * @param[out] paramName, set to parameter name.
 * @return, OF_Result_Success in case of success, others in case of failure.
 */
OF_EFFECT_EXPORT_API OF_Result OF_GetFilterParamName(
    OFHandle contextID,                         /* [in] */
    OFHandle filterID,                          /* [in] */
    OFUInt32 paramIndex,                        /* [in] */
    OFName paramName);                          /* [out] */

OF_EFFECT_EXPORT_API OF_Result OF_GetFilterParamData(
    OFHandle contextID,                         /* [in] */
    OFHandle filterID,                          /* [in] */
    const char* paramName,                      /* [in] */
    OF_Param** param);                          /* [out] */

OF_EFFECT_EXPORT_API OF_Result OF_SetFilterParamData(
    OFHandle contextID,                         /* [in] */
    OFHandle filterID,                          /* [in] */
    const char* paramName,                      /* [in] */
    OF_Param* param);                           /* [in] */

OF_EFFECT_EXPORT_API OF_Result OF_SetFilterExtData(
    OFHandle contextID,                         /* [in] */
    OFHandle filterID,                          /* [in] */
    const void* extData);                       /* [in] */

OF_EFFECT_EXPORT_API OF_Result OF_GetFilterExtData(
    OFHandle contextID,                         /* [in] */
    OFHandle filterID,                          /* [in] */
    void* extData,                              /* [out] */
    OFUInt32 extDataSize);                      /* [in] */

OF_EFFECT_EXPORT_API OF_Result OF_GetFilterType(
    OFHandle contextID,                         /* [in] */
    OFHandle filterID,                          /* [in] */
    char* type);                                /* [out] */

OF_EFFECT_EXPORT_API OF_Result OF_GetFilterSubType(
    OFHandle contextID,                         /* [in] */
    OFHandle filterID,                          /* [in] */
    char* subType);                             /* [out] */

OF_EFFECT_EXPORT_API OF_Result OF_CreateEffectFromFile(
    OFHandle contextID,                         /* [in] */
    const char* filePath,                       /* [in] */
    OFHandle* effectID,                         /* [out] */
    const char* resDir);                        /* [in] */

OF_EFFECT_EXPORT_API OF_Result OF_CreateEffectFromData(
    OFHandle contextID,                         /* [in] */
    const char* json,                           /* [in] */
    OFHandle* effectID,                         /* [out] */
    const char* resDir);                        /* [in] */

OF_EFFECT_EXPORT_API OF_Result OF_CreateEffectFromPackage(
    OFHandle contextID,                         /* [in] */
    const char* packagePath,                    /* [in] */
    const char* effectName,                     /* [in] */
    const char* decryptKey,                     /* [in] */
    OFHandle* effectID);                        /* [out] */

OF_EFFECT_EXPORT_API OF_Result OF_CreateEffectFromPackageMemory(
    OFHandle contextID,                         /* [in] */
    const char* stream,                         /* [in] */
    OFInt32 streamSize,                         /* [in] */
    const char* effectName,                     /* [in] */
    const char* decryptKey,                     /* [in] */
    OFHandle* effectID);                        /* [out] */

OF_EFFECT_EXPORT_API OF_Result OF_CreateEmptyEffect(
    OFHandle contextID,                         /* [in] */
    OFHandle* effectID,                         /* [out] */
    const char* resDir);                        /* [in] */

OF_EFFECT_EXPORT_API OF_Result OF_UpdateEffectFromFile(
    OFHandle contextID,                         /* [in] */
    OFHandle effectID,                          /* [in] */
    const char* filePath,                       /* [in] */
    const char* resDir);                        /* [in] */

OF_EFFECT_EXPORT_API OF_Result OF_UpdateEffectFromData(
    OFHandle contextID,                         /* [in] */
    OFHandle effectID,                          /* [in] */
    const char* json,                           /* [in] */
    const char* resDir);                        /* [in] */

OF_EFFECT_EXPORT_API OF_Result OF_VerifyEffectFile(
    const char* filePath,                       /* [in] */
    const char* resDir);                        /* [in] */

OF_EFFECT_EXPORT_API OF_Result OF_VerifyEffectData(
    const char* json,                           /* [in] */
    const char* resDir);                        /* [in] */

OF_EFFECT_EXPORT_API OF_Result OF_DestroyEffect(
    OFHandle contextID,                         /* [in] */
    OFHandle effectID);                         /* [in] */

OF_EFFECT_EXPORT_API OF_Result OF_SaveEffectToFile(
    OFHandle contextID,                         /* [in] */
    OFHandle effectID,                          /* [in] */
    const char* filePath);                      /* [in] */

OF_EFFECT_EXPORT_API OF_Result OF_SaveEffectToData(
    OFHandle contextID,                         /* [in] */
    OFHandle effectID,                          /* [in] */
    char* json,                                 /* [out] */
    OFInt32 maxJsonSize);                       /* [in] */

OF_EFFECT_EXPORT_API OF_Result OF_ApplyFrame(
    OFHandle contextID,                         /* [in] */
    OFHandle handlerID,                         /* [in] */
    OF_Texture* inputArray,                     /* [in] */
    OFUInt32 inputCount,                        /* [in] */
    OF_Texture* outputArray,                    /* [in] */
    OFUInt32 outputCount,                       /* [in] */
    OF_FrameData* frameData);                   /* [in] */

OF_EFFECT_EXPORT_API OF_Result OF_ApplyFrameBatch(
    OFHandle contextID,                         /* [in] */
    const OFHandle* idList,                     /* [in] */
    OFUInt32 idCount,                           /* [in] */
    OF_Texture* inputArray,                     /* [in] */
    OFUInt32 inputCount,                        /* [in] */
    OF_Texture* outputArray,                    /* [in] */
    OFUInt32 outputCount,                       /* [in] */
    OF_FrameData* frameData,                    /* [in] */
    OF_Result *resultList,                      /* [in] */
    OFUInt32 resultCount);                      /* [in] */

OF_EFFECT_EXPORT_API OF_Result OF_ApplyFrameTransit(
    OFHandle contextID,                         /* [in] */
    OFHandle fromHandlerID,                     /* [in] */
    OFHandle toHandlerID,                       /* [in] */
    OFHandle transitEffectID,                   /* [in] */
    float percent,                              /* [in] */
    OF_Texture* inputArray,                     /* [in] */
    OFUInt32 inputCount,                        /* [in] */
    OF_Texture* outputArray,                    /* [in] */
    OFUInt32 outputCount,                       /* [in] */
    OF_FrameData* frameData);                   /* [in] */

OF_EFFECT_EXPORT_API OF_Result OF_ApplyFrameBatchTransit(
    OFHandle contextID,                         /* [in] */
    OFHandle* fromHandlerIDArray,               /* [in] */
    OFUInt32 fromHandlerIDCount,                /* [in] */
    OFHandle* toHandlerIDArray,                 /* [in] */
    OFUInt32 toHandlerIDCount,                  /* [in] */
    OFHandle transitEffectID,                   /* [in] */
    float percent,                              /* [in] */
    OF_Texture* inputArray,                     /* [in] */
    OFUInt32 inputCount,                        /* [in] */
    OF_Texture* outputArray,                    /* [in] */
    OFUInt32 outputCount,                       /* [in] */
    OF_FrameData* frameData,                    /* [in] */
    OF_Result* resultArray,                     /* [out] */
    OFUInt32 resultCount);                      /* [in] */

OF_EFFECT_EXPORT_API OF_Result OF_IsAllowSameInputOutput(
    OFHandle contextID,                         /* [in] */
    OFHandle handlerID,                         /* [in] */
    OFBool* isAllow);                           /* [out] */

OF_EFFECT_EXPORT_API OF_Result OF_GetRequiredInputCount(
    OFHandle contextID,                         /* [in] */
    OFHandle handlerID,                         /* [in] */
    OFUInt32* count);                           /* [out] */

OF_EFFECT_EXPORT_API OF_Result OF_GetRequiredOutputCount(
    OFHandle contextID,                         /* [in] */
    OFHandle handlerID,                         /* [in] */
    OFUInt32* count);                           /* [out] */

OF_EFFECT_EXPORT_API OF_Result OF_GetRequiredFrameData(
    OFHandle contextID,                         /* [in] */
    const OFHandle* idList,                     /* [in] */
    OFUInt32 idCount,                           /* [in] */
    OFUInt32* requiredFrameData);               /* [out] */

OF_EFFECT_EXPORT_API OF_Result OF_GetEffectInfo(
    OFHandle contextID,                         /* [in] */
    OFHandle effectID,                          /* [in] */
    OF_EffectInfo* effectInfo);                 /* [out] */

OF_EFFECT_EXPORT_API OF_Result OF_IsSupportEffect(
    OFHandle contextID,                         /* [in] */
    const char* filePath);                      /* [in] */

OF_EFFECT_EXPORT_API OF_Result OF_GetProfilerInfo(
    OFHandle contextID,                         /* [in] */
    OFHandle handlerID,                         /* [in] */
    OF_ProfilerInfo* profilerInfo);             /* [out] */

OF_EFFECT_EXPORT_API OF_Result OF_SendMessage(
    OFHandle contextID,                         /* [in] */
    OFHandle handlerID,                         /* [in] */
    const char* msg,                            /* [in] */
    char* resultMsgBuffer,                      /* [out] */
    OFUInt32 resultMsgBufferSize);              /* [in] */

/* Deprecated: Use OF_SetMessageCallback2 replace. */
OF_EFFECT_EXPORT_API OF_Result OF_SetMessageCallback(
    OFHandle contextID,                         /* [in] */
    OFHandle handlerID,                         /* [in] */
    void (*callback)(OFHandle, const char*, char*, OFUInt32)); /* [in] */

typedef struct _OF_MessageCallback {
    void* userData;
    void (*onMessageBack)(void *userData, const char* msg, char* resultMsg, OFUInt32 resultMsgSize);
    void (*onUserDataDestory)(void *userData);
} OF_MessageCallback;

OF_EFFECT_EXPORT_API OF_Result OF_SetMessageCallback2(
    OFHandle contextID,                         /* [in] */
    OFHandle handlerID,                         /* [in] */
    OF_MessageCallback callback);               /* [in] */

OF_EFFECT_EXPORT_API OF_Result OF_CreateEffectAnimator(
    OFHandle contextID,                         /* [in] */
    OFHandle effectID,                          /* [in] */
    const char* animatorType,                   /* [in] */
    const void* extData,                        /* [in] */
    OFHandle* animatorID);                      /* [out] */

OF_EFFECT_EXPORT_API OF_Result OF_DestoryEffectAnimator(
    OFHandle contextID,                         /* [in] */
    OFHandle effectID,                          /* [in] */
    OFHandle animatorID);                       /* [in] */

OF_EFFECT_EXPORT_API OF_Result OF_SetEffectAnimatorExtData(
    OFHandle contextID,                         /* [in] */
    OFHandle effectID,                          /* [in] */
    OFHandle animatorID,                        /* [in] */
    const void* extData);                       /* [in] */

OF_EFFECT_EXPORT_API OF_Result OF_GetEffectAnimatorExtData(
    OFHandle contextID,                         /* [in] */
    OFHandle effectID,                          /* [in] */
    OFHandle animatorID,                        /* [in] */
    void* extData,                              /* [out] */
    OFUInt32 extDataSize);                      /* [in] */

OF_EFFECT_EXPORT_API OF_Result OF_GetEffectAnimatorType(
    OFHandle contextID,                         /* [in] */
    OFHandle effectID,                          /* [in] */
    OFHandle animatorID,                        /* [in] */
    char* type);                                /* [out] */

OF_EFFECT_EXPORT_API OF_Result OF_CreateEffectScene(
    OFHandle contextID,                         /* [in] */
    OFHandle effectID,                          /* [in] */
    const char* sceneType,                      /* [in] */
    const void* extData,                        /* [in] */
    OFHandle* sceneID);                         /* [out] */

OF_EFFECT_EXPORT_API OF_Result OF_DestoryEffectScene(
    OFHandle contextID,                         /* [in] */
    OFHandle effectID,                          /* [in] */
    OFHandle sceneID);                          /* [in] */

OF_EFFECT_EXPORT_API OF_Result OF_SetEffectDuration(
    OFHandle contextID,                         /* [in] */
    OFHandle effectID,                          /* [in] */
    OFUInt64 duration);                         /* [in] */

OF_EFFECT_EXPORT_API OF_Result OF_SetEffectSceneDuration(
    OFHandle contextID,                         /* [in] */
    OFHandle effectID,                          /* [in] */
    OFHandle sceneID,                           /* [in] */
    OFUInt64 duration);                        /* [in] */

OF_EFFECT_EXPORT_API OF_Result OF_GetEffectSceneDuration(
    OFHandle contextID,                         /* [in] */
    OFHandle effectID,                          /* [in] */
    OFHandle sceneID,                           /* [in] */
    OFUInt64 *duration);                        /* [out] */

OF_EFFECT_EXPORT_API OF_Result OF_SetEffectSceneTriggers(
    OFHandle contextID,                         /* [in] */
    OFHandle effectID,                          /* [in] */
    OFHandle sceneID,                           /* [in] */
    OFUInt32 triggers);                        /* [in] */

OF_EFFECT_EXPORT_API OF_Result OF_GetEffectSceneTriggers(
    OFHandle contextID,                         /* [in] */
    OFHandle effectID,                          /* [in] */
    OFHandle sceneID,                           /* [in] */
    OFUInt32 *triggers);                        /* [out] */

OF_EFFECT_EXPORT_API OF_Result OF_SetEffectSceneSpecturemRange(
    OFHandle contextID,                         /* [in] */
    OFHandle effectID,                          /* [in] */
    OFHandle sceneID,                           /* [in] */
    OFUInt16 begin,                             /* [in] */
    OFUInt16 end);                              /* [in] */

OF_EFFECT_EXPORT_API OF_Result OF_GetEffectSceneSpecturemRange(
    OFHandle contextID,                         /* [in] */
    OFHandle effectID,                          /* [in] */
    OFHandle sceneID,                           /* [in] */
    OFUInt16 *begin,                            /* [in] */
    OFUInt16 *end);                             /* [out] */

OF_EFFECT_EXPORT_API OF_Result OF_SetEffectSceneExtData(
    OFHandle contextID,                         /* [in] */
    OFHandle effectID,                          /* [in] */
    OFHandle sceneID,                           /* [in] */
    const void* extData);                       /* [in] */

OF_EFFECT_EXPORT_API OF_Result OF_GetEffectSceneExtData(
    OFHandle contextID,                         /* [in] */
    OFHandle effectID,                          /* [in] */
    OFHandle sceneID,                           /* [in] */
    void* extData,                              /* [out] */
    OFUInt32 extDataSize);                      /* [in] */

OF_EFFECT_EXPORT_API OF_Result OF_GetEffectSceneType(
    OFHandle contextID,                         /* [in] */
    OFHandle effectID,                          /* [in] */
    OFHandle sceneID,                           /* [in] */
    char* type);                                /* [out] */

OF_EFFECT_EXPORT_API OF_Result OF_RestartEffectAnimation(
    OFHandle contextID,                         /* [in] */
    OFHandle effectID);                         /* [in] */

OF_EFFECT_EXPORT_API OF_Result OF_PauseEffectAnimation(
    OFHandle contextID,                         /* [in] */
    OFHandle effectID);                         /* [in] */

OF_EFFECT_EXPORT_API OF_Result OF_PlayEffectAnimation(
    OFHandle contextID,                         /* [in] */
    OFHandle effectID);                         /* [in] */

OF_EFFECT_EXPORT_API OF_Result OF_SeekEffectAnimation(
    OFHandle contextID,                         /* [in] */
    OFHandle effectID,                          /* [in] */
    OFUInt64 timestamp);                        /* [in] */

OF_EFFECT_EXPORT_API OF_Result OF_GetEffectAnimationTimeStamp(
    OFHandle contextID,                         /* [in] */
    OFHandle effectID,                          /* [in] */
    OFUInt64* timestamp);                       /* [out] */

OF_EFFECT_EXPORT_API OF_Result OF_SetEffectVersion(
    OFHandle contextID,                         /* [in] */
    OFHandle effectID,                          /* [in] */
    const char* version);                       /* [in] */

OF_EFFECT_EXPORT_API OF_Result OF_SetEffectTrackData(
    OFHandle contextID,                         /* [in] */
    OFHandle effectID,                          /* [in] */
    OF_EffectTrackData* data,                   /* [in] */
    OFUInt32 count);                            /* [in] */

OF_EFFECT_EXPORT_API OF_Result OF_SaveTexture(
    OFHandle contextID,                         /* [in] */
    OF_Texture* tex,                            /* [in] */
    const char* filePath);                      /* [in] */

OF_EFFECT_EXPORT_API OF_Result OF_CopyTexture(
    OFHandle contextID,                         /* [in] */
    OF_Texture* input,                          /* [in] */
    OF_Texture* output);                        /* [in] */

OF_EFFECT_EXPORT_API OF_Result OF_MirrorTexture(
    OFHandle contextID,                         /* [in] */
    OF_Texture* input,                          /* [in] */
    OF_Texture* output,                         /* [out] */
    OFBool horizontally,                        /* [in] */
    OFBool vertically);                         /* [in] */

OF_EFFECT_EXPORT_API OF_Result OF_SwapRGB(
    OFHandle contextID,                         /* [in] */
    OF_Texture* input,                          /* [in] */
    OF_Texture* output);                        /* [in] */

OF_EFFECT_EXPORT_API OF_Result OF_ConvertYUVToRGB(
    OFHandle contextID,                         /* [in] */
    OF_Texture* inTexY,                         /* [in] */
    OF_Texture* inTexUV,                        /* [in] */
    OF_Texture* output);                        /* [in] */

OF_EFFECT_EXPORT_API OF_Result OF_BlendTexture(
    OFHandle contextID,                         /* [in] */
    OF_Texture* base,                           /* [in] */
    OF_Texture* blend,                          /* [in] */
    OF_Texture* output,                         /* [in] */
    OF_BlendMode blendMode,                     /* [in] */
    float opacity);                             /* [in] */

OF_EFFECT_EXPORT_API OF_Result OF_ApplyDoubleEffectYUV(
    OFHandle contextID,                         /* [in] */
    OFHandle effect0ID,                         /* [in] */
    OFHandle effect1ID,                         /* [in] */
    float ratio,                                /* [in] */
    OFBool isVertical,                          /* [in] */
    OF_Texture* inTexY,                         /* [in] */
    OF_Texture* inTexUV,                        /* [in] */
    OF_Texture* outTex,                         /* [in] */
    OF_Texture* debugTex,                       /* [in] */
    OF_FrameData* frameData);                   /* [in] */

OF_EFFECT_EXPORT_API OF_Result OF_ApplyDoubleEffectRGBA(
    OFHandle contextID,                         /* [in] */
    OFHandle effect0ID,                         /* [in] */
    OFHandle effect1ID,                         /* [in] */
    float ratio,                                /* [in] */
    OFBool isVertical,                          /* [in] */
    OF_Texture* inTex,                          /* [in] */
    OF_Texture* outTex,                         /* [in] */
    OF_Texture* debugTex,                       /* [in] */
    OF_FrameData* frameData);                   /* [in] */

OF_EFFECT_EXPORT_API OF_Result OF_CreateEmptyGame(
    OFHandle contextID,                         /* [in] */
    OFHandle* gameID,                           /* [out] */
    const char* resDir);                        /* [in] */

OF_EFFECT_EXPORT_API OF_Result OF_CreateGameFromFile(
    OFHandle contextID,                         /* [in] */
    const char* filePath,                       /* [in] */
    OFHandle* gameID,                           /* [out] */
    const char* resDir);                        /* [in] */

OF_EFFECT_EXPORT_API OF_Result OF_UpdateGameFromFile(
    OFHandle contextID,                         /* [in] */
    OFHandle gameID,                            /* [in] */
    const char* filePath,                       /* [in] */
    const char* resDir);                        /* [in] */

OF_EFFECT_EXPORT_API OF_Result OF_DestroyGame(
    OFHandle contextID,                         /* [in] */
    OFHandle gameID);                           /* [in] */

OF_EFFECT_EXPORT_API OF_Result OF_GetGameType(
    OFHandle contextID,                         /* [in] */
    OFHandle gameID,                            /* [in] */
    char* typeBuffer,                           /* [out] */
    OFInt32 typeBufferSize);                    /* [in] */

OF_EFFECT_EXPORT_API OF_Result OF_StartGame(
    OFHandle contextID,                         /* [in] */
    OFHandle gameID);                           /* [in] */

OF_EFFECT_EXPORT_API OF_Result OF_PauseGame(
    OFHandle contextID,                         /* [in] */
    OFHandle gameID);                           /* [in] */

OF_EFFECT_EXPORT_API OF_Result OF_ResumeGame(
    OFHandle contextID,                         /* [in] */
    OFHandle gameID);                           /* [in] */

OF_EFFECT_EXPORT_API OF_Result OF_StopGame(
    OFHandle contextID,                         /* [in] */
    OFHandle gameID);                           /* [in] */

OF_EFFECT_EXPORT_API OF_Result OF_SendGameEventJson(
    OFHandle contextID,                         /* [in] */
    OFHandle gameID,                            /* [in] */
    const char* json);                          /* [in] */

OF_EFFECT_EXPORT_API OF_Result OF_SetGameEventCallbackJson(
    OFHandle contextID,                         /* [in] */
    OFHandle gameID,                            /* [in] */
    void(*callback)(OFInt32, const char*));     /* [in] */
    
OF_EFFECT_EXPORT_API OF_Result OF_CopyCStrToTempBuffer(
    OFHandle contextID,         /* [in]  */
    const char* str,            /* [in]  */
    OFUInt32 len,               /* [in]  */
    const char** resultStr);    /* [out] */

OF_EFFECT_EXPORT_API OF_Result OF_ClearCachedResource(
    OFHandle contextID);                        /* [in] */

/* This api is just for Venus 278 points transform*/
OF_EFFECT_EXPORT_API OF_Result OF_TransVenusSingleFacePointsToOFFacePoints(
    float * venusFacePoints,                            /* [in] */ 
    OFUInt32 venusFacePointsCount,                     /* [in] */ 
    OFUInt8 faceIndex,                                 /* [in] */
    OF_FrameData * frameData);                        /* [in] */

/* prepare frame data extData */
OF_EFFECT_EXPORT_API OF_FrameData* OF_PrepareFramedataExtra(
    OFHandle contextID,                         /* [in] */
    OFHandle handlerID,                         /* [in] */
    OF_FrameData* frameData,                    /* [in] */
    OF_FrameData* newFrameData);                /* [in] */

    
/************************************************************************/
/* Following api is deprecated, just for compatible with old app.       */
/************************************************************************/
/* Deprecated: Use OF_ApplyFrame replace. */
OF_EFFECT_EXPORT_API OF_Result OF_ApplyYUV(
    OFHandle contextID,                         /* [in] */
    OFHandle handlerID,                         /* [in] */
    OF_Texture* inTexY,                         /* [in] */
    OF_Texture* inTexUV,                        /* [in] */
    OF_Texture* outTex,                         /* [in] */
    OF_Texture* debugTex,                       /* [in] */
    OF_FrameData* frameData);                   /* [in] */

/* Deprecated: Use OF_ApplyFrame replace. */
OF_EFFECT_EXPORT_API OF_Result OF_ApplyRGBA(
    OFHandle contextID,                         /* [in] */
    OFHandle handlerID,                         /* [in] */
    OF_Texture* inTex,                          /* [in] */
    OF_Texture* outTex,                         /* [in] */
    OF_Texture* debugTex,                       /* [in] */
    OF_FrameData* frameData);                   /* [in] */

/* Deprecated: Use OF_ApplyFrameBatch replace. */
OF_EFFECT_EXPORT_API OF_Result OF_ApplyBatchYUV(
    OFHandle contextID,                         /* [in] */
    const OFHandle* idList,                     /* [in] */
    OFUInt32 idCount,                           /* [in] */
    OF_Texture* inTexY,                         /* [in] */
    OF_Texture* inTexUV,                        /* [in] */
    OF_Texture* outTex,                         /* [in] */
    OF_Texture* debugTex,                       /* [in] */
    OF_FrameData* frameData);                   /* [in] */

/* Deprecated: Use OF_ApplyFrameBatch replace. */
OF_EFFECT_EXPORT_API OF_Result OF_ApplyBatchRGBA(
    OFHandle contextID,                         /* [in] */
    const OFHandle* idList,                     /* [in] */
    OFUInt32 idCount,                           /* [in] */
    OF_Texture* inTex,                          /* [in] */
    OF_Texture* outTex,                         /* [in] */
    OF_Texture* debugTex,                       /* [in] */
    OF_FrameData* frameData);                   /* [in] */

/* Deprecated: Use OF_ApplyFrame replace. */
OF_EFFECT_EXPORT_API OF_Result OF_ApplyFilterYUV(
    OFHandle contextID,                         /* [in] */
    OFHandle filterID,                          /* [in] */
    OF_Texture* inTexY,                         /* [in] */
    OF_Texture* inTexUV,                        /* [in] */
    OF_Texture* outTex,                         /* [in] */
    OF_Texture* debugTex,                       /* [in] */
    OF_FrameData* frameData);                   /* [in] */

/* Deprecated: Use OF_ApplyFrame replace. */
OF_EFFECT_EXPORT_API OF_Result OF_ApplyFilterRGBA(
    OFHandle contextID,                         /* [in] */
    OFHandle filterID,                          /* [in] */
    OF_Texture* inTex,                          /* [in] */
    OF_Texture* outTex,                         /* [in] */
    OF_Texture* debugTex,                       /* [in] */
    OF_FrameData* frameData);                   /* [in] */

/* Deprecated: Use OF_ApplyFrame replace. */
OF_EFFECT_EXPORT_API OF_Result OF_ApplyEffectYUV(
    OFHandle contextID,                         /* [in] */
    OFHandle effectID,                          /* [in] */
    OF_Texture* inTexY,                         /* [in] */
    OF_Texture* inTexUV,                        /* [in] */
    OF_Texture* outTex,                         /* [in] */
    OF_Texture* debugTex,                       /* [in] */
    OF_FrameData* frameData);                   /* [in] */

/* Deprecated: Use OF_ApplyFrame replace. */
OF_EFFECT_EXPORT_API OF_Result OF_ApplyEffectRGBA(
    OFHandle contextID,                         /* [in] */
    OFHandle effectID,                          /* [in] */
    OF_Texture* inTex,                          /* [in] */
    OF_Texture* outTex,                         /* [in] */
    OF_Texture* debugTex,                       /* [in] */
    OF_FrameData* frameData);                   /* [in] */

/* Deprecated: Use OF_ApplyFrame replace. */
OF_EFFECT_EXPORT_API OF_Result OF_ApplyGameYUV(
    OFHandle contextID,                         /* [in] */
    OFHandle gameID,                            /* [in] */
    OF_Texture* inTexY,                         /* [in] */
    OF_Texture* inTexUV,                        /* [in] */
    OF_Texture* outTex,                         /* [in] */
    OF_Texture* debugTex,                       /* [in] */
    OF_FrameData* frameData);                   /* [in] */

/* Deprecated: Use OF_ApplyFrame replace. */
OF_EFFECT_EXPORT_API OF_Result OF_ApplyGameRGBA(
    OFHandle contextID,                         /* [in] */
    OFHandle gameID,                            /* [in] */
    OF_Texture* inTex,                          /* [in] */
    OF_Texture* outTex,                         /* [in] */
    OF_Texture* debugTex,                       /* [in] */
    OF_FrameData* frameData);                   /* [in] */

/* Deprecated: only used for FaceDanceGame, FoodGame, DartGame, ARDanceGame. */
OF_EFFECT_EXPORT_API OF_Result OF_SendGameEvent(
    OFHandle contextID,                         /* [in] */
    OFHandle gameID,                            /* [in] */
    OFInt32 gameEventID,                        /* [in] */
    void* gameEventData);                       /* [in] */

/* Deprecated: only used for FaceDanceGame, FoodGame, DartGame, ARDanceGame. */
OF_EFFECT_EXPORT_API OF_Result OF_SetGameData(
    OFHandle contextID,                         /* [in] */
    OFHandle gameID,                            /* [in] */
    void* gameData);                            /* [in] */

/* Deprecated: Use OF_GetFilterParamCount replace. */
OF_EFFECT_EXPORT_API OF_Result OF_GetFilterParamfCount(
    OFHandle contextID,                         /* [in] */
    OFHandle filterID,                          /* [in] */
    OFUInt32* paramfCount);                     /* [out] */

/* Deprecated: Use OF_GetFilterParamData replace. */
OF_EFFECT_EXPORT_API OF_Result OF_GetFilterParamfData(
    OFHandle contextID,                         /* [in] */
    OFHandle filterID,                          /* [in] */
    OFInt32 index,                              /* [in] */
    OF_Paramf* param);                          /* [out] */

/* Deprecated: Use OF_GetFilterParamData replace. */
OF_EFFECT_EXPORT_API OF_Result OF_GetFilterParamf(
    OFHandle contextID,                         /* [in] */
    OFHandle filterID,                          /* [in] */
    OFInt32 index,                              /* [in] */
    float* val);                                /* [out] */

/* Deprecated: Use OF_GetFilterParamData replace. */
OF_EFFECT_EXPORT_API OF_Result OF_GetFilterDefaultParamf(
    OFHandle contextID,                         /* [in] */
    OFHandle filterID,                          /* [in] */
    OFInt32 index,                              /* [in] */
    float* val);                                /* [out] */

/* Deprecated: Use OF_SetFilterParamData replace. */
OF_EFFECT_EXPORT_API OF_Result OF_SetFilterParamf(
    OFHandle contextID,                         /* [in] */
    OFHandle filterID,                          /* [in] */
    OFInt32 index,                              /* [in] */
    float val);                                 /* [in] */

/* Deprecated: Use OF_GetFilterParamData replace. */
OF_EFFECT_EXPORT_API OF_Result OF_GetFilterParamfRange(
    OFHandle contextID,                         /* [in] */
    OFHandle filterID,                          /* [in] */
    OFInt32 index,                              /* [in] */
    float* minVal,                              /* [out] */
    float* maxVal);                             /* [out] */

/* Deprecated. */
OF_EFFECT_EXPORT_API OF_Result OF_Remove3dArNode(
    OFHandle contextID,                         /* [in] */
    OFHandle filterID,                          /* [in] */
    OFUInt32 sceneID,                           /* [in] */
    OFUInt32 nodeID);                           /* [in] */

/* Deprecated. */
OF_EFFECT_EXPORT_API OF_Result OF_SetRadom3dArNode(
    OFHandle contextID,                         /* [in] */
    OFHandle filterID,                          /* [in] */
    OFUInt32 rdmNum);                           /* [in] */

#define OF_ApplyYUV_RGBA    OF_ApplyFilterYUV

#ifdef __cplusplus
}
#endif

#endif /* _OF_EFFECT_H_ */
