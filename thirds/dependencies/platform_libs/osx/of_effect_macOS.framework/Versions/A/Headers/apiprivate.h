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

#include "orangefilter.h"
#include <assert.h>
#include <cstdlib>
#include <string>
#include <vector>

/************************************************************************/
/* Macros for utilites.                                                 */
/************************************************************************/

//#define OF_NO_ORANGE3D
//#define OF_MODULE_NO_FREETYPE
//#define OF_NO_WEBM
//#define OF_MODULE_NO_BULLET
//#define OF_NO_LUA

#define OF_ARRAY_SIZE(a) (sizeof(a) / sizeof(a[0]))

//
// A macro to disallow the copy constructor and operator= functions
// This should be used in the private: declarations for a class
//
#define OF_DISALLOW_COPY_AND_ASSIGN(TypeName) \
private:                                      \
    TypeName(const TypeName &);               \
    TypeName &operator=(const TypeName &);

//
// A macro to disallow all the implicit constructors, namely the
// default constructor, copy constructor and operator= functions.
//
// This should be used in the protected: declarations for a class
// that wants to prevent anyone from instantiating it. This is
// especially useful for classes containing only static methods.
//
#define OF_DISALLOW_IMPLICIT_CONSTRUCTORS(TypeName) \
protected:                                          \
    TypeName();                                     \
    OF_DISALLOW_COPY_AND_ASSIGN(TypeName);

//
// A macro to disallow the explicit destructor.
//
#define OF_DISALLOW_EXPLICIT_DESTRUCTORS(TypeName) \
protected:                                         \
    virtual ~TypeName();

//
// Macro for simplifying to delete a pointer and set it OF_NULL.
//
#define OF_SAFE_FREE(obj) \
    if (obj)              \
    {                     \
        free(obj);        \
        obj = OF_NULL;    \
    }

#define OF_SAFE_DELETE(obj) \
    if (obj)                \
    {                       \
        delete obj;         \
        obj = OF_NULL;      \
    }

#define OF_SAFE_RELEASE(obj) \
    if ((obj))               \
    {                        \
        (obj)->decRef();     \
        (obj) = OF_NULL;     \
    }

#define OF_SAFE_DELETE_ARRAY(arr) \
    if (arr)                      \
    {                             \
        delete[] arr;             \
        arr = OF_NULL;            \
    }

//
// Macros for impl.
//
#define OF_DECLARE_PRIVATE(Class)      \
private:                               \
    friend class Class##Private;       \
    Class##Private *_d_ptr;            \
    Class##Private &of_d()             \
    {                                  \
        return *_d_ptr;                \
    }                                  \
    const Class##Private &of_d() const \
    {                                  \
        return *_d_ptr;                \
    }

#define OF_DECLARE_PUBLIC(Class)      \
public:                               \
    friend class Class;               \
    Class *_p_ptr;                    \
    inline void setPublic(Class *ptr) \
    {                                 \
        _p_ptr = ptr;                 \
    }                                 \
    Class &of_p()                     \
    {                                 \
        return *_p_ptr;               \
    }                                 \
    const Class &of_p() const         \
    {                                 \
        return *_p_ptr;               \
    }

#define OF_INIT_PRIVATE(Class)     \
    _d_ptr = new Class##Private(); \
    _d_ptr->setPublic(this);

#define OF_D(Class)             \
    Class##Private &d = of_d(); \
    d
#define OF_D_C(Class)                 \
    const Class##Private &d = of_d(); \
    d

#define OF_P(Class)    \
    Class &p = of_p(); \
    p
#define OF_P_C(Class)        \
    const Class &p = of_p(); \
    p

#define OF_FINI_PRIVATE() \
    delete _d_ptr;        \
    _d_ptr = 0;

#define OF_UNUSED(x) (void)x
#if defined(_DEBUG) || defined(DEBUG)
#define OF_ASSERT(x) assert(x)
#else
#define OF_ASSERT(x)
#endif // DEBUG

#define OF_CHECK_GL_ERROR()                                                          \
    {                                                                                \
        int err = glGetError();                                                      \
        if (err != 0)                                                                \
        {                                                                            \
            OF_LOGE("glGetError: 0x%x in file:%s line:%d", err, __FILE__, __LINE__); \
        }                                                                            \
    }

//
// Filter.
//
#define OF_DECLARE_FILTER_PRIVATE(Class)                   \
    OF_DECLARE_PRIVATE(Class)                              \
public:                                                    \
    static BaseFilter *create##Class()                     \
    {                                                      \
        return BaseFilter::createFilter<Class>();          \
    }                                                      \
    static void destroy##Class(BaseFilter *filter)         \
    {                                                      \
        Class *realFilter = dynamic_cast<Class *>(filter); \
        OF_ASSERT(realFilter);                             \
        if (realFilter)                                    \
        {                                                  \
            BaseFilter::destroyFilter<Class>(realFilter);  \
        }                                                  \
    }

//
// Animator.
//
#define OF_DECLARE_ANIMATOR_PRIVATE(Class)                      \
    OF_DECLARE_PRIVATE(Class)                                   \
public:                                                         \
    static BaseAnimator *create##Class()                        \
    {                                                           \
        return BaseAnimator::createAnimator<Class>();           \
    }                                                           \
    static void destroy##Class(BaseAnimator *animator)          \
    {                                                           \
        Class *realAnimator = dynamic_cast<Class *>(animator);  \
        OF_ASSERT(realAnimator);                                \
        if (realAnimator)                                       \
        {                                                       \
            BaseAnimator::destroyAnimator<Class>(realAnimator); \
        }                                                       \
    }

//
// Scene.
//
#define OF_DECLARE_SCENE_PRIVATE(Class)                  \
    OF_DECLARE_PRIVATE(Class)                            \
public:                                                  \
    static BaseScene *create##Class()                    \
    {                                                    \
        return BaseScene::createScene<Class>();          \
    }                                                    \
    static void destroy##Class(BaseScene *scene)         \
    {                                                    \
        Class *realScene = dynamic_cast<Class *>(scene); \
        OF_ASSERT(realScene);                            \
        if (realScene)                                   \
        {                                                \
            BaseScene::destroyScene<Class>(realScene);   \
        }                                                \
    }

//
// Game.
//
#define OF_DECLARE_GAME_PRIVATE(Class)                 \
    OF_DECLARE_PRIVATE(Class)                          \
public:                                                \
    static Game *create##Class()                       \
    {                                                  \
        return Game::createGame<Class>();              \
    }                                                  \
    static void destroy##Class(Game *game)             \
    {                                                  \
        Class *realGame = dynamic_cast<Class *>(game); \
        OF_ASSERT(realGame);                           \
        if (realGame)                                  \
        {                                              \
            Game::destroyGame<Class>(realGame);        \
        }                                              \
    }

//
// Bit op.
//
#define IS_INCLUDED_FLAG_8(flags, flag) ((flags & flag) == 0x00 ? false : true)
#define IS_INCLUDED_FLAG_16(flags, flag) ((flags & flag) == 0x0000 ? false : true)
#define IS_INCLUDED_FLAG_32(flags, flag) ((flags & flag) == 0x00000000 ? false : true)
#define IS_INCLUDED_FLAG_64(flags, flag) ((flags & flag) == 0x0000000000000000 ? false : true)
#define BTI_MSK(bit) (1 << (bit))
#define BIT_SET(x, bit) ((x) |= BTI_MSK(bit))
#define BIT_CLR(x, bit) ((x) &= ~BTI_MSK(bit))
#define BIT_TST(x, bit) ((x)&BTI_MSK(bit))

//
// Image op.
//
#define BYTES_PER_LINE(width, bpp) ((((width) * (bpp)*8 + 31) >> 5) << 2) // bytes per scanline (must be multiple of 4)
#define BUFFER_OFFSET(i) ((char *)NULL + (i))

//
// Miscellaneous.
//
#ifdef __APPLE__
#if TARGET_IPHONE_SIMULATOR || TARGET_OS_IPHONE
#ifndef OF_IOS
#define OF_IOS 1
#endif
#else
#ifndef OF_MAC
#define OF_MAC 1
#endif
#endif
#endif

namespace OrangeFilter
{

/************************************************************************/
/* Interfaces                                                           */
/************************************************************************/

//
// Enum the preallocated vertex attribute.
//
enum
{
    // Index 0 will be used as Position.
    VERTEX_ATTRIB_POSITION,
    // Index 1 will be used as Color.
    VERTEX_ATTRIB_COLOR,
    // Index 2 will be used as Tex coord unit 0.
    VERTEX_ATTRIB_TEX_COORD,
    // Index 3 will be used as Tex coord unit 1.
    VERTEX_ATTRIB_TEX_COORD1,
    // Index 4 will be used as Tex coord unit 2.
    VERTEX_ATTRIB_TEX_COORD2,
    // Index 5 will be used as Tex coord unit 3.
    VERTEX_ATTRIB_TEX_COORD3,
    // Index 6 will be used as Normal.
    VERTEX_ATTRIB_NORMAL,
    // Index 7 will be used as Blend weight for hardware skin.
    VERTEX_ATTRIB_BLEND_WEIGHT,
    // Index 8 will be used as Blend index.
    VERTEX_ATTRIB_BLEND_INDEX,
    // Index 9 will be used as tangent.
    VERTEX_ATTRIB_TANGENT,
    // Index 10 will be used as Binormal.
    VERTEX_ATTRIB_BINORMAL,
    VERTEX_ATTRIB_MAX,

    // backward compatibility
    VERTEX_ATTRIB_TEX_COORDS = VERTEX_ATTRIB_TEX_COORD,
};

//
// For compatible with old version Effect's mirror mode, EffectVersion >= 4 does not need to care of this.
//
enum MirrorMode
{
    MirrorMode_NotSet,
    MirrorMode_ForceMirror,
    MirrorMode_NotMirror,
};

// clang-format off
#define KShaderPassKey_DefaultError                     "default_error_pass"
#define KShaderPassKey_Copy                             "copy_pass"
#define KShaderPassKey_Copy2D                           "copy_2d_pass"
#define KShaderPassKey_Copy_OES                         "copy_oes_pass"
//#define KShaderPassKey_FireCircle                       "firecircle_pass"
#define KShaderPassKey_Pupil                            "pupil_draw_pass"
#define KShaderPassKey_Mirror0                          "mirror0_pass"
#define KShaderPassKey_Brightness                       "brightness_pass"
#define KShaderPassKey_Exposure                         "exposure_pass"
#define KShaderPassKey_Contrast                         "contrast_pass"
#define KShaderPassKey_Mirror                           "mirror_pass"
#define KShaderPassKey_Crop                             "crop_pass"
#define KShaderPassKey_CompositeTex                     "composite_tex_pass"
#define KShaderPassKey_Nv12ToRgb                        "nv12torgb_pass"
#define KShaderPassKey_YuvToRgba                        "yuvtorgba_pass"
#define KShaderPassKey_SwapRGB                          "swaprgb_pass"
#define KShaderPassKey_RgbToYuv                         "rgbtoyuv_pass"
#define KShaderPassKey_YuvToUv                          "yuvtouv_pass"
#define KShaderPassKey_DenoiseInit                      "denoise_init_pass"
#define KShaderPassKey_DenoiseUpdate                    "denoise_update_pass"
#define KShaderPassKey_DenoiseShow                      "denoise_show_pass"
#define KShaderPassKey_SkinInit                         "skin_init_pass"
#define KShaderPassKey_SkinUpdate                       "skin_update_pass"
#define KShaderPassKey_Skin0                            "skin0_pass"
#define KShaderPassKey_Skin1                            "skin1_pass"

#define KShaderPassKey_Beauty0Guided0                   "beauty0_guided0_pass"
#define KShaderPassKey_Beauty0Guided1                   "beauty0_guided1_pass"
#define KShaderPassKey_Beauty0Gauss0                    "beauty0_gauss0_pass"
#define KShaderPassKey_Beauty0Gauss1                    "beauty0_gauss1_pass"

#define KShaderPassKey_Beauty1Stage0                    "beauty1_stage0_pass"
#define KShaderPassKey_Beauty1Stage1                    "beauty1_stage1_pass"
#define KShaderPassKey_Beauty1Stage2                    "beauty1_stage2_pass"
#define KShaderPassKey_Beauty1Stage3                    "beauty1_stage3_pass"
#define KShaderPassKey_Beauty1Debug                     "beauty1_debug_pass"

#define KShaderPassKey_Beauty2Skin                      "beauty2_skin_pass"
#define KShaderPassKey_Beauty2Blur0                     "beauty2_blur0_pass"
#define KShaderPassKey_Beauty2Blur1                     "beauty2_blur1_pass"
#define KShaderPassKey_Beauty2Blur2                     "beauty2_blur2_pass"
#define KShaderPassKey_Beauty2Guided0                   "beauty2_guided0_pass"
#define KShaderPassKey_Beauty2Guided1                   "beauty2_guided1_pass"
#define KShaderPassKey_Beauty2Gauss0                    "beauty2_gauss0_pass"
#define KShaderPassKey_Beauty2Gauss1                    "beauty2_gauss1_pass"

#define KShaderPassKey_Beauty3State0                    "beauty3_stage0_pass"
#define KShaderPassKey_Beauty3State1                    "beauty3_stage1_pass"
#define KShaderPassKey_Beauty3State2                    "beauty3_stage2_pass"
#define KShaderPassKey_Beauty3State3                    "beauty3_stage3_pass"

#define KShaderPassKey_Beauty4Guided0                   "beauty4_guide0_pass"
#define KShaderPassKey_Beauty4Guided1                   "beauty4_guide1_pass"
#define KShaderPassKey_Beauty4Gauss0                    "beauty4_gauss0_pass"
#define KShaderPassKey_Beauty4Gauss1                    "beauty4_gauss1_pass"

#define KShaderPassKey_Beauty5Guided0                   "beauty5_guided0_pass"
#define KShaderPassKey_Beauty5Guided1                   "beauty5_guided1_pass"
#define KShaderPassKey_Beauty5Gauss0                    "beauty5_gauss0_pass"
#define KShaderPassKey_Beauty5Gauss1                    "beauty5_gauss1_pass"
#define KShaderPassKey_Beauty5Guided0_NoLoop            "beauty5_guided0_noloop_pass"
#define KShaderPassKey_Beauty5Guided1_NoLoop            "beauty5_guided1_noloop_pass"
//#define KShaderPassKey_Beauty5Guided1_Yuv_Device_Level_Low "beauty5_guided1_yuv_device_level_low_pass"
//#define KShaderPassKey_Beauty5Guided1_NoLoop_Yuv_Device_Level_Low "beauty5_guided1_noloop_yuv_device_level_low_pass"
//#define KShaderPassKey_Beauty5Guided0_Rgb_Device_Level_Low "beauty5_guided0_rgb_device_level_low_pass"
//#define KShaderPassKey_Beauty5Guided1_Rgb_Device_Level_Low "beauty5_guided1_rgb_device_level_low_pass"
//#define KShaderPassKey_Beauty5Guided0_NoLoop_Rgb_Device_Level_Low "beauty5_guided0_noloop_rgb_device_level_low_pass"
//#define KShaderPassKey_Beauty5Guided1_NoLoop_Rgb_Device_Level_Low "beauty5_guided1_noloop_rgb_device_level_low_pass"
#define KShaderPassKey_Beauty5RGBFastGuided0            "beauty5_rgb_fast_guided0_pass"
#define KShaderPassKey_Beauty5RGBFastGuided1            "beauty5_rgb_fast_guided1_pass"
#define KShaderPassKey_Beauty5RGBFastGuided2            "beauty5_rgb_fast_guided2_pass"


//#define KShaderPassKey_Beauty6State0                    "beauty6_stage0_pass"
//#define KShaderPassKey_Beauty6State1                    "beauty6_stage1_pass"
//#define KShaderPassKey_Beauty6State2                    "beauty6_stage2_pass"
//
//#define KShaderPassKey_Beauty7Bilateral                 "beauty7_bilateral_pass"
//#define KShaderPassKey_Beauty7Guided0                   "beauty7_guided0_pass"
//#define KShaderPassKey_Beauty7Guided1                   "beauty7_guided1_pass"
//#define KShaderPassKey_Beauty7Gauss0                    "beauty7_gauss0_pass"
//#define KShaderPassKey_Beauty7Gauss1                    "beauty7_gauss1_pass"
//#define KShaderPassKey_Beauty7Merge                     "beauty7_merge_pass"

#define KShaderPassKey_Beauty8Guided0                   "beauty8_guided0_pass"
#define KShaderPassKey_Beauty8Guided1                   "beauty8_guided1_pass"
#define KShaderPassKey_Beauty8Gauss0                    "beauty8_gauss0_pass"
#define KShaderPassKey_Beauty8Smooth                    "beauty8_smooth_pass"
#define KShaderPassKey_Beauty8Merge                     "beauty8_merge_pass"

#define KShaderPassKey_Beauty9Guided0                   "beauty9_guided0_pass"
#define KShaderPassKey_Beauty9Guided1                   "beauty9_guided1_pass"
#define KShaderPassKey_Beauty9Gauss0                    "beauty9_gauss0_pass"
#define KShaderPassKey_Beauty9Gauss1                    "beauty9_gauss1_pass"
#define KShaderPassKey_Beauty9Smooth                    "beauty9_smooth_pass"
#define KShaderPassKey_Beauty9Merge                     "beauty9_merge_pass"

#define KShaderPassKey_Beauty10Stage0                   "beauty10_stage0_pass"
#define KShaderPassKey_Beauty10Stage0_Device_Level_1    "beauty10_stage0_device_low_pass"

#define KShaderPassKey_BeautySoftBlur                   "beautysoftblur_pass"
#define KShdaerPassKey_BeautySoftVar                    "beautysoftvar_pass"
#define KShdaerPassKey_BeautySoftAlphaBlur              "beautysoftalphablur_pass"
#define KShderPassKey_BeautySoftBlend                   "beautysoftblend_pass"

#define KShaderPassKey_LookupTable64                    "lookupTable64_pass"
#define KShaderPassKey_LookupTable512                   "lookupTable512_pass"
#define KShaderPassKey_LookupTable3D                    "lookupTable3D_pass"
#define KShaderPassKey_MaskLookupTable64                "mask_lookupTable64_pass"
#define KShaderPassKey_MaskLookupTable512               "mask_lookupTable512_pass"
#define KShaderPassKey_LipsMaskLookupTable64            "lips_mask_lookuptable_64_pass"
#define KShaderPassKey_Interpolate2Texture              "interpolate_2texture_pass"
#define KShaderPassKey_Curve                            "curve_pass"
#define KShaderPassKey_SelectiveColor                   "selectivecolor_pass"
#define KShaderPassKey_Blend_Normal                     "blend_normal_pass"
#define KShaderPassKey_Blend_Substract                  "blend_substract_pass"
#define KShaderPassKey_Blend_Multiply                   "blend_multiply_pass"
#define KShaderPassKey_Blend_Overlay                    "blend_overlay_pass"
#define KShaderPassKey_Blend_SoftLight                  "blend_softlight_pass"
#define KShaderPassKey_Blend_HardLight                  "blend_hardlight_pass"
#define KShaderPassKey_Blend_LinearLight                "blend_linearlight_pass"
#define KShaderPassKey_Blend_Screen                     "blend_screen_pass"
#define KShaderPassKey_Blend_ColorBurn                  "blend_colorburn_pass"
#define KShaderPassKey_Blend_Difference                 "blend_difference_pass"
#define KShaderPassKey_Blend_ColorDodge                 "blend_colordodge_pass"
#define KShaderPassKey_Blend_LinearDodge                "blend_lineardodge_pass"
#define KShaderPassKey_Blend_HSLColor                   "blend_hslcolor_pass"
#define KShaderPassKey_Blend_HSLHue                     "blend_hslhue_pass"
#define KShaderPassKey_Blend_HSLSaturate                "blend_hslsaturate_pass"
#define KShaderPassKey_Blend_HSLLuminosity              "blend_hslluminosity_pass"
#define KShaderPassKey_Blend_Darken                     "blend_darken_pass"
#define KShaderPassKey_Blend_DarkerColor                "blend_darkencolor_pass"
#define KShaderPassKey_Blend_Lighten                    "blend_lighten_pass"
#define KShaderPassKey_Blend_LightenColor               "blend_lightencolor_pass"
#define KShaderPassKey_Blend_RGBOverlay                 "blend_rgboverlay_pass"
#define KShaderPassKey_LuminosityWeaken_Normal          "luminosity_weaken_normal"
#define KShaderPassKey_LuminosityWeaken_Multiply        "luminosity_weaken_multiply"
#define KShaderPassKey_LuminosityWeaken_LinearDodge     "luminosity_weaken_linearDodge"
#define KShaderPassKey_LuminosityWeaken_SoftLight       "luminosity_weaken_softlight"
#define KShaderPassKey_LuminosityWeaken_HardLight       "luminosity_weaken_hardlight"
#define KShaderPassKey_LuminosityWeaken_Screen          "luminosity_weaken_screen"
#define KShaderPassKey_SegmentBlend                     "masking_blend_pass"
#define KShaderPassKey_CloneMask                        "masking_clone_pass"
#define KShaderPassKey_SimpleUISprite                   "simple_uisprite_pass"
#define KShaderPassKey_MattingBlend                     "mattingblend_pass"
#define KShaderPassKey_PointDraw                        "pointdraw_pass"
#define KShaderPassKey_PointDraw2D                      "pointdraw_2d_pass"
//#define KShaderPassKey_SimpleThinFace                   "simple_thinface_pass"
#define KShaderPassKey_BasicThinFace                    "basic_thinface_pass"
#define KShaderPassKey_BasicThinFaceLow                 "basic_thinface_pass_low"
#define KShaderPassKey_OptimizedBasicThinFace           "optimized_basic_thinface_pass"
#define KShaderPassKey_BigHead                          "bighead_pass"
#define KShaderPassKey_FaceLiquify                      "faceliquify_pass"
#define KShaderPassKey_FaceLifting_OPt                  "facelifting_opt_pass"
#define KShaderPassKey_FaceLifting_OPt_Separate0        "facelifting_opt_separate_pass0"
#define KShaderPassKey_FaceLifting_OPt_Separate1        "facelifting_opt_separate_pass1"
#define KShaderPassKey_FaceLifting_OPt_Separate2        "facelifting_opt_separate_pass2"
#define KShaderPassKey_I18nFaceLifting                  "i18nfacelifting_pass"
#define KShaderPassKey_I18nFaceLifting_Separate0        "i18nfacelifting_separate_pass0"
#define KShaderPassKey_I18nFaceLifting_Separate1        "i18nfacelifting_separate_pass1"
#define KShaderPassKey_I18nFaceLifting_Separate2        "i18nfacelifting_separate_pass2"
#define KShaderPassKey_MTFaceLifting_OPt                "mtfacelifting_opt_pass"
#define KShaderPassKey_FaceLiftingThinFace              "facelifting_thinface_pass"
#define KShaderPassKey_FaceLiftingSmallFace             "facelifting_smallface_pass"
#define KShaderPassKey_FaceLiftingEye                   "facelifting_eye_pass"
#define KShaderPassKey_FaceLiftingNose                  "facelifting_nose_pass"
#define KShaderPassKey_FaceLiftingLongNose              "facelifting_longnose_pass"
#define KShaderPassKey_FaceLiftingMouth                 "facelifting_mouth_pass"
#define KShaderPassKey_FaceLiftingSquashedFace          "facelifting_squashedface_pass"
#define KShaderPassKey_FaceLiftingChin                  "facelifting_chin_pass"
#define KShaderPassKey_FaceLiftingForehead              "facelifting_forehead_pass"
#define KShaderPassKey_FaceLiftingWideForehead          "facelifting_wideforehead_pass"
#define KShaderPassKey_ComiclFaceLiftingRuhua           "facelifting_ruhua_pass"
#define KShaderPassKey_ComiclFaceLiftingErKang          "facelifting_erkang_pass"
#define KShaderPassKey_ComiclFaceLiftingErKang_Sep0     "facelifting_erkang_sep0_pass"
#define KShaderPassKey_ComiclFaceLiftingErKang_Sep1     "facelifting_erkang_sep1_pass"
#define KShaderPassKey_ComiclFaceLiftingHuLuWa          "facelifting_huluwa_pass"
#define KShaderPassKey_FleshyFace                       "fleshyface_pass"
#define KShaderPassKey_MaskBlendDraw                    "maskblend_pass"
#define KShaderPassKey_MaskWeightBlendDraw              "maskweightblend_pass"
#define KShaderPassKey_MakeupPupilBlend                 "makeuppupilblend_pass"
#define KShaderPassKey_MaskBlur                         "maskblur_pass"
#define KShaderPassKey_MaskBlur_NoLoop                  "maskblur_noloop_pass"
#define KShaderPassKey_Luminance                        "luminance_pass"
#define KShaderPassKey_LuminanceThreshold               "luminancethreshold_pass"
#define KShaderPassKey_GaussianBlurPosition             "gaussianblurposition_pass"
#define KShaderPassKey_SobelEdgeDetection               "sobeledgedetection_pass"
#define KShaderPassKey_SobelColorEdge                   "sobelcoloredge_pass"
#define KShaderPassKey_LaplacianEdgeDetection           "laplacianedgedetection_pass"
#define KShaderPassKey_RobertsEdgeDetection             "robertsedgedetection_pass"
#define KShaderPassKey_DirectionalNonMaximumSuppression "directionalnonmaximumsuppression_pass"
#define KShaderPassKey_DirectionalSobelEdgeDetection    "directional_sobeledgedetection_pass"
#define KShaderPassKey_WeakPixelInclusion               "weakpixelinclusion_pass"
#define KShaderPassKey_HighLightShadow                  "highlightshadow_pass"
#define KShaderPassKey_HighLightShadowTint              "highlightshadowtint_pass"
#define KShaderPassKey_Sketch                           "sketch_pass"
#define KShaderPassKey_Shake                            "shake_pass"
#define KShaderPassKey_RandomShake                      "randomshake_pass"
#define KShaderPassKey_Toon                             "toon_pass"
#define KShaderPassKey_NoiseTV0                         "noisetv_pass0"
#define KShaderPassKey_NoiseTV1                         "noisetv_pass1"
#define KShaderPassKey_NoiseCRT                         "noise_crt_pass"
#define KShaderPassKey_ShakeNoise                       "shakenoise_pass"
#define KShaderPassKey_DistortionShake                  "distortionshake_pass"
#define KSahderPassKey_Browse                           "browse_pass"
#define KShaderPassKey_ComponentMove                    "componentmove_pass"
#define KShaderPassKey_RandNoise                        "randnoise_pass"
//#define KShaderPassKey_Slices                           "slices_pass"
#define KShaderPassKey_Ripple                           "ripple_pass"
//#define KShaderPassKey_Honeycomb                        "honeycomb_pass"
#define KShaderPassKey_StretchDistortion                "stretchdistortion_pass"
//#define KShaderPassKey_Wave0                            "wave0_pass"
//#define KShaderPassKey_Wave1                            "wave1_pass"
#define KShaderPassKey_Mesh3d                           "mesh3d_pass"
#define KShaderPassKey_Mesh3dTex                        "mesh3d_tex_pass"
#define KShaderPassKey_Mesh3dTex_etc1                   "mesh3d_tex_etc1_pass"
#define KShaderPassKey_Mesh3dPhong                      "mesh3d_phong_pass"
#define KShaderPassKey_Mesh3dPBR                        "mesh3d_pbr_pass"
#define KShaderPassKey_Mesh3dSkin                       "mesh3d_skin_pass"
#define KShaderPassKey_Mesh3dPBRCutout                  "mesh3d_pbr_cutout_pass"
#define KShaderPassKey_Mesh3dSkinCutout                 "mesh3d_skin_cutout_pass"
#define KShaderPassKey_ParticleAdd                      "particle_add_pass"
#define KShaderPassKey_ParticleBlend                    "particle_blend_pass"
#define KShaderPassKey_ParticleAddInstance              "particle_add_instance_pass"
#define KShaderPassKey_UISprite                         "ui_sprite_pass"
#define KShaderPassKey_UISpriteInstance                 "ui_sprite_instance_pass"
#define KShaderPassKey_PaintTex                         "paint_tex_pass"
#define KShaderPassKey_Particle                         "particle_pass"
#define KShaderPassKey_ParticleInstance                 "particle_instance_pass"
#define KShaderPassKey_Lines                            "lines_pass"
#define KShaderPassKey_TextureColor                     "texture_color_pass"
#define KShaderPassKey_ColorPacking                     "colorpacking_pass"
#define KShaderPassKey_GlassSphere                      "glass_sphere_pass"
//#define KShaderPassKey_Dilation_Radius_One              "dilation_radius_one_pass"
//#define KShaderPassKey_Dilation_Radius_Two              "dilation_radius_two_pass"
//#define KShaderPassKey_Dilation_Radius_Three            "dilation_radius_three_pass"
//#define KShaderPassKey_Dilation_Radius_Four             "dilation_radius_four_pass"
#define KShaderPassKey_HarrisCornerDetection            "harriscornerdetection_pass"
#define KShaderPassKey_GaussBlurRange                   "gaussblurrange_pass"
#define KShaderPassKey_MotionBlur                       "motionblur_pass"
#define KShaderPassKey_ZoomBlur                         "zoomblur_pass"
#define KShaderPassKey_SpiralBlur                       "spiralblur_pass"
#define KShaderPassKey_TranslationBlur                  "translationblur_pass"
#define KShaderPassKey_BoxBlur                          "boxblur_pass"
#define KShaderPassKey_GaussianBlur                     "gaussianblur_pass"
#define KSahderPassKey_GaussianMaskBlur                 "gaussianmaskblur_pass"
#define KShaderPassKey_BlurRange                        "blurrange_pass"
#define KShaderPassKey_UnsharpMask                      "unsharpmask_pass"
#define KShaderPassKey_Pixellate                        "pixellate_pass"
#define KShaderPassKey_PolarPixellate                   "polarpixellate_pass"
#define KShaderPassKey_PixellatePosition                "pixellateposition_pass"
#define KShaderPassKey_Text                             "text_pass"
#define KShaderPassKey_TextTTF                          "text_ttf_pass"
#define KShaderPassKey_TextSequence                     "text_sequence_pass"
#define KShaderPassKey_TextFade                         "text_fade_pass"
#define KShaderPassKey_SoulBlend                        "soulblend_pass"
#define KShaserPassKey_Phantom                          "phantom_pass"
#define KShaserPassKey_PhantomBlend                     "phantomblend_pass"
#define KShaderPassKey_InvertMask                       "invertmask_pass"
#define KShaderPassKey_InvertColor                      "invertcolor_pass"
#define KShaderPassKey_SoftGlowBlend                    "softglowblend_pass"
#define KShaderPassKey_Vignette                         "vignette_pass"
#define KShaderPassKey_SVGATex                          "svga_tex_pass"
#define KShaderPassKey_SVGABlend                        "svga_blend_pass"
#define KShaderPassKey_SVGA2Tex                         "svga2_tex_pass"
#define KShaderPassKey_SVGA2UI                          "svga2_ui_pass"
#define KShaderPassKey_RGB2Lab                          "rgb2lab_pass"
#define KShaderPassKey_Lab2RGB                          "lab2lab_pass"
#define KShaderPassKey_FaceSwapBlend                    "faceswapblend_pass"

#define KDuration_Loop   0

#define KObjectType_Effect                              "Effect"
#define KObjectType_Game                                "Game"
// clang-format on

//
// For compatible with old version Particle System EffectVersion >= 4.9 does not need to care of this.
//
#define OF_USE_EARLY_VERSION_PARTICLE


struct Config
{
    OF_RendererType renderType;             /* OF_ConfigKey_RenderType */
    OFPath globalResDir;                    /* OF_ConfigKey_GlobalResDir */
    OFBool isSaveJsonToPretty;              /* OF_ConfigKey_IsSaveJsonPretty */
    OFBool isMirror;                        /* OF_ConfigKey_IsMirror */
    OFInt32 screenWidth;                    /* OF_ConfigKey_ScreenWidth */
    OFInt32 screenHeight;                   /* OF_ConfigKey_ScreenHeight */
    OF_ScreenOrientation screenOrientation; /* OF_ConfigKey_ScreenOrientation */
    OFBool isSaveEffectLatestVersion;       /* OF_ConfigKey_SaveEffectLatestVersion */
    float animation2DPlaySpeed;             /* OF_ConfigKey_Animation2DPlaySpeed */
    OF_DeviceLevel deviceLevel;             /* OF_ConfigKey_DeviceLevel */
    OFBool isAsyncLoadIO;                   /* OF_ConfigKey_IsAsyncLoadIO */
    OFBool isProfilerOn;                    /* OF_ConfigKey_IsProfilerOn */
};

class GraphicsEngine;
class BaseFilter;
class Effect;
class Program;
class Texture;
class PixelBuffer;

struct Render
{
    virtual ~Render()
    {
    }
    virtual void draw(Program *program, bool bVFlip = false) = 0;
};

struct ITexture
{
    virtual void updateRef(int width, int height, int format, int target, OFUInt32 textureId) = 0;
    virtual OFUInt32 textureID() const = 0;
    virtual OFUInt32 target() const = 0;
    virtual OFInt32 format() const = 0;
    virtual OFUInt32 bindFBO(OFUInt32 fbo) = 0;
    virtual OFInt32 width() const = 0;
    virtual OFInt32 height() const = 0;
    virtual void tearDown() = 0;
    virtual void toData(OF_Texture *tex) const = 0;
    virtual ~ITexture() = 0;
};

//
// Filter Registration Information.
//
class BaseFilter;
typedef BaseFilter *(*FnCreateFilter)();
typedef void (*FnDestroyFilter)(BaseFilter *);

struct FilterRegInfo
{
    std::string filterType;
    FnCreateFilter fnCreateFilter;
    FnDestroyFilter fnDestroyFilter;
};

//
// Animator Registration Information.
//
class BaseAnimator;
typedef BaseAnimator *(*FnCreateAnimator)();
typedef void (*FnDestroyAnimator)(BaseAnimator *);

struct AnimatorRegInfo
{
    std::string animatorType;
    FnCreateAnimator fnCreateAnimator;
    FnDestroyAnimator fnDestroyAnimator;
};

//
// Scene Registration Information.
//
class BaseScene;
typedef BaseScene *(*FnCreateScene)();
typedef void (*FnDestroyScene)(BaseScene *);

struct SceneRegInfo
{
    std::string sceneType;
    FnCreateScene fnCreateScene;
    FnDestroyScene fnDestroyScene;
};

struct TextureData
{
    std::string _path;   // in
    OFInt32 _filterType; // in
    OFInt32 _wrapType;   // in
    Texture **_pTexture; // out
};

//
// Game Registration Information.
//
class Game;
typedef Game *(*FnCreateGame)();
typedef void (*FnDestroyGame)(Game *);

struct GameRegInfo
{
    std::string gameType;
    FnCreateGame fnCreateGame;
    FnDestroyGame fnDestroyGame;
};

//
// Engine global.
//
OF_EFFECT_API GraphicsEngine *GetGraphicsEngine();
OF_EFFECT_API bool RegisterFilter(const char *filterType,
                                  FnCreateFilter fnCreateFilter,
                                  FnDestroyFilter fnDestroyFilter);
OF_EFFECT_API bool UnRegisterFilter(const char *filterType);
OF_EFFECT_API bool RegisterAnimator(const char *animatorType,
                                    FnCreateAnimator fnCreateAnimator,
                                    FnDestroyAnimator fnDestroyAnimator);
OF_EFFECT_API bool UnRegisterAnimator(const char *animatorType);
OF_EFFECT_API bool RegisterScene(const char *sceneType, FnCreateScene fnCreateScene, FnDestroyScene fnDestroyScene);
OF_EFFECT_API bool UnRegisterScene(const char *sceneType);
OF_EFFECT_API bool RegisterGame(const char *gameType, FnCreateGame fnCreateGame, FnDestroyGame fnDestroyGame);
OF_EFFECT_API bool UnRegisterGame(const char *gameType);
//
// Shader Program.
//
OF_EFFECT_API Program *CreateCustomShaderPass(OFHandle contextID, const char *vs, const char *fs);
OF_EFFECT_API void DestroyCustomShaderPass(OFHandle contextID, Program *program);

//
// Texture.
//
OF_EFFECT_API Texture *
CreateTexture(OFHandle contextID, int width, int height, int format, int target, unsigned char *data);
OF_EFFECT_API Texture *
CreateReferenceTexture(OFHandle contextID, int width, int height, int format, int target, OFUInt32 textureId);
OF_EFFECT_API void DestroyTexture(OFHandle contextID, Texture *texture);

OF_EFFECT_API PixelBuffer *CreatePixelBuffer(OFHandle contextID, int width, int height, int format);
OF_EFFECT_API void DestroyPixelBuffer(OFHandle contextID, PixelBuffer *pixelBuf);

OF_EFFECT_API OFUInt32 GetFilterParamCount(OFHandle contextID, OFHandle filterID);
OF_EFFECT_API OF_Param *GetFilterParam(OFHandle contextID, OFHandle filterID, OFInt32 index);
OF_EFFECT_API void SetFilterParam(OFHandle contextID, OFHandle filterID, OFInt32 index, const OF_Param *param);
OF_EFFECT_API void SetFilterParamIsCurve(OFHandle contextID, OFHandle filterID, OFInt32 index, bool bIsCurve);
OF_EFFECT_API void *GetFilterFloatParamCurve(OFHandle contextID, OFHandle filterID, OFInt32 index);
OF_EFFECT_API void *GetFilterColorParamCurve(OFHandle contextID, OFHandle filterID, OFInt32 index);

OF_EFFECT_API void
SetEffectInputList(OFHandle contextID, OFHandle effectID, OFUInt32 inputCount, const OF_InputMediaData *inputList);
OF_EFFECT_API void
GetEffectInputList(OFHandle contextID, OFHandle effectID, OFUInt32 &inputCount, OF_InputMediaData *inputList);
OF_EFFECT_API void GetLocusUserImage(OFHandle contextID, OFHandle effectID, OF_Locus_UserImage *userimageinfo);
OF_EFFECT_API void SetLocusUserImage(OFHandle contextID, OFHandle effectID, const OF_Locus_UserImage *userimageinfo);
OF_EFFECT_API void SetEffectTrackTime(OFHandle contextID, OFHandle effectID, OFUInt64 trackTime);
OF_EFFECT_API void SetEffectLoop(OFHandle contextID, OFHandle effectID, bool bLoop);
OF_EFFECT_API void SetEffectDuration(OFHandle contextID, OFHandle effectID, OFUInt64 duration);
OF_EFFECT_API void SetEffectAudioName(OFHandle contextID, OFHandle effectID, const char *name);
OF_EFFECT_API void SetEffectFadeout(OFHandle contextID, OFHandle effectID, bool isFadeout, float start);
OF_EFFECT_API void SetEffectMirrorMode(OFHandle contextID, OFHandle effectID, MirrorMode mirrorMode);
OF_EFFECT_API const char *GetEffectResDir(OFHandle contextID, OFHandle effectID);
OF_EFFECT_API OFUInt32 GetEffectParamfCount(OFHandle contextID, OFHandle effectID);
OF_EFFECT_API void AddEffectParamf(OFHandle contextID, OFHandle effectID, OFUInt32 filterIndex, OFUInt32 paramfIndex);
OF_EFFECT_API void
SetEffectParamf(OFHandle contextID, OFHandle effectID, OFUInt32 index, OFUInt32 filterIndex, OFUInt32 paramfIndex);
OF_EFFECT_API const OF_EffectParamfData *GetEffectParamf(OFHandle contextID, OFHandle effectID, OFUInt32 index);
OF_EFFECT_API void RemoveEffectParamf(OFHandle contextID, OFHandle effectID, OFUInt32 index);
OF_EFFECT_API OF_Result TransitEffect(OFHandle contextID,
                                      OF_Texture *from,
                                      OF_Texture *to,
                                      OFHandle transitEffectID,
                                      float percent,
                                      OF_Texture *outputArray,
                                      OFUInt32 outputCount,
                                      OF_FrameData *frameData);

OF_EFFECT_API void SetFilterFaceExclusive(OFHandle contextID, OFHandle filterID, bool ex);
OF_EFFECT_API bool IsFilterFaceExclusive(OFHandle contextID, OFHandle filterID);
OF_EFFECT_API void SetFilterTransformFlag(OFHandle contextID, OFHandle filterID, OFUInt32 flag);
OF_EFFECT_API OFUInt32 GetFilterTransformFlag(OFHandle contextID, OFHandle filterID);
OF_EFFECT_API void SetFilterDuration(OFHandle contextID, OFHandle filterID, OFUInt64 dur);
OF_EFFECT_API OFUInt64 GetFilterDuration(OFHandle contextID, OFHandle filterID);

OF_EFFECT_API OFHandle AddEffectFilter(OFHandle contextID, OFHandle effectID, const char *filterType);
OF_EFFECT_API void RemoveEffectFilter(OFHandle contextID, OFHandle effectID, OFHandle filterID);
OF_EFFECT_API OFHandle GetEffectFilter(OFHandle contextID, OFHandle effectID, OFUInt32 index);
OF_EFFECT_API OFHandle GetEffectFilterFromUUID(OFHandle contextID, OFHandle effectID, OFUUID uuid);
OF_EFFECT_API const char *GetEffectFilterUUID(OFHandle contextID, OFHandle effectID, OFHandle filterID);
OF_EFFECT_API void
MoveUpEffectFilter(OFHandle contextID, OFHandle effectID, OFHandle *filterList, OFUInt32 filterListCount);
OF_EFFECT_API void
MoveDownEffectFilter(OFHandle contextID, OFHandle effectID, OFHandle *filterList, OFUInt32 filterListCount);
OF_EFFECT_API void SetEffectFilterFreeze(OFHandle contextID, OFHandle effectID, OFHandle filterID, bool isFreeze);
OF_EFFECT_API bool IsEffectFilterFreeze(OFHandle contextID, OFHandle effectID, OFHandle filterID);

OF_EFFECT_API void SetEffectAnimatorFreeze(OFHandle contextID, OFHandle effectID, OFHandle sceneID, bool isFreeze);
OF_EFFECT_API bool IsEffectAnimatorFreeze(OFHandle contextID, OFHandle effectID, OFHandle sceneID);

OF_EFFECT_API OFHandle GetEffectSceneFromUUID(OFHandle contextID, OFHandle effectID, OFUUID uuid);
OF_EFFECT_API const char *GetEffectSceneUUID(OFHandle contextID, OFHandle effectID, OFHandle sceneID);
OF_EFFECT_API void SetEffectSceneFreeze(OFHandle contextID, OFHandle effectID, OFHandle sceneID, bool isFreeze);
OF_EFFECT_API bool IsEffectSceneFreeze(OFHandle contextID, OFHandle effectID, OFHandle sceneID);
OF_EFFECT_API void SetEffectSceneDuration(OFHandle contextID, OFHandle effectID, OFHandle sceneID, OFUInt64 duration);
OF_EFFECT_API OFUInt64 GetEffectSceneDuration(OFHandle contextID, OFHandle effectID, OFHandle sceneID);
OF_EFFECT_API void SetEffectSceneTriggerOnce(OFHandle contextID, OFHandle effectID, OFHandle sceneID, bool flag);
OF_EFFECT_API bool IsEffectSceneTriggerOnce(OFHandle contextID, OFHandle effectID, OFHandle sceneID);
OF_EFFECT_API void SetEffectSceneResetFilterTime(OFHandle contextID, OFHandle effectID, OFHandle sceneID, bool flag);
OF_EFFECT_API bool IsEffectSceneResetFilterTime(OFHandle contextID, OFHandle effectID, OFHandle sceneID);
OF_EFFECT_API void
MoveUpEffectScene(OFHandle contextID, OFHandle effectID, OFHandle *sceneList, OFUInt32 sceneListCount);
OF_EFFECT_API void
MoveDownEffectScene(OFHandle contextID, OFHandle effectID, OFHandle *sceneList, OFUInt32 sceneListCount);
OF_EFFECT_API void
SetFilterTimeRange(OFHandle contextID, OFHandle effectID, OFUInt32 filterIndex, OFUInt64 uBeginTime, OFUInt64 uEndTime);

OF_EFFECT_API bool
JudgeFilterInEffectScenes(OFHandle contextID, OFHandle effectID, OFHandle filterID, bool isRemove = false);
OF_EFFECT_API bool
JudgeFilterInEffectAnimators(OFHandle contextID, OFHandle effectID, OFHandle filterID, bool isRemove = false);

OF_EFFECT_API const Config *GetGlobalConfig(OFHandle contextID);
OF_EFFECT_API bool
SavePng(const char *path, const void *data, OFInt32 width, OFInt32 height, OF_PixelFormat pixelFormat);
OF_EFFECT_API OFUInt8 *LoadPng(const std::string path, OFInt32 &width, OFInt32 &height);
OF_EFFECT_API bool SaveJpeg(const char *path,
                            const void *data,
                            OFInt32 width,
                            OFInt32 height,
                            OF_PixelFormat pixelFormat,
                            int quality = 75 /*quality is between 1 and 100.*/);
OF_EFFECT_API OFUInt8 *LoadJpeg(const std::string path, OFInt32 &width, OFInt32 &height);

OF_EFFECT_API OF_Result CreateSVGAFromFile(OFHandle contextID,
                                           const char *filePath,
                                           OFHandle *svgaID,
                                           const char *resDir);
OF_EFFECT_API OF_Result
ApplySVGA(OFHandle contextID, OFHandle svgaID, OF_Texture *outTex, OF_Rect rect, OFInt32 curFrame);
OF_EFFECT_API OF_Result DestroySVGA(OFHandle contextID, OFHandle svgaID);
OF_EFFECT_API OFUInt32 GetSVGAviewWidth(OFHandle contextID, OFHandle svgaID);
OF_EFFECT_API OFUInt32 GetSVGAviewHeight(OFHandle contextID, OFHandle svgaID);
OF_EFFECT_API OFUInt32 GetSVGAFPS(OFHandle contextID, OFHandle svgaID);
OF_EFFECT_API OFUInt32 GetSVGAframes(OFHandle contextID, OFHandle svgaID);
OF_EFFECT_API OF_Result SetSVGAFromFile(OFHandle contextID,
                                        OFHandle filterID,
                                        const char *filePath,
                                        const char *resDir);
OF_EFFECT_API OF_Result SetSVGAImageForSprite(OFHandle contextID,
                                              OFHandle svgaID,
                                              const char *imageKey,
                                              const char *imagePath);

//
// Ar Camera
//
OF_EFFECT_API OF_Result SetArCameraParams(OFHandle arCameraId, float params[], int paramCnt);

//
// Custom lua library extension of of_effect.
//
typedef void (*FuncRegisterLuaLib)(void *luaState);
OF_EFFECT_API OF_Result RegisterCustomLuaLib(const char *name, FuncRegisterLuaLib funcRegisterLuaLib);
OF_EFFECT_API OF_Result UnRegisterCustomLuaLib(const char *name);
//
// Effect Api :lock
//

OF_EFFECT_API void EffectApiLock();
OF_EFFECT_API void EffectApiUnlock();
/************************************************************************/
/* Classes for utilites.                                                */
/************************************************************************/

class Noncopyable
{
protected:
    Noncopyable() = default;
    ~Noncopyable() = default;

    Noncopyable(const Noncopyable &) = delete;
    Noncopyable &operator=(const Noncopyable &) = delete;
};

template <class T>
class CachedList
{
public:
    OFUInt32 add(const T &obj)
    {
        if (!_freeList.empty())
        {
            OFUInt32 index = _freeList.back();
            _freeList.pop_back();
            _objects[index] = obj;
            return index + 1;
        }
        else
        {
            _objects.push_back(obj);
            return (OFUInt32)_objects.size();
        }
    }

    void remove(OFUInt32 handle)
    {
        OF_ASSERT(handle > 0 && handle <= _objects.size());
        _objects[handle - 1] = T(); // Destruct and replace with default object
        _freeList.push_back(handle - 1);
    }

    T &getRef(OFUInt32 handle)
    {
        OF_ASSERT(handle > 0 && handle <= _objects.size());
        return _objects[handle - 1];
    }

    const T &getRef(OFUInt32 handle) const
    {
        OF_ASSERT(handle > 0 && handle <= _objects.size());
        return _objects[handle - 1];
    }

    friend class GLDevice;

private:
    std::vector<T> _objects;
    std::vector<OFUInt32> _freeList;
};

template <class T>
class CachedPtrList
{
public:
    OFUInt32 add(const T &obj)
    {
        if (!_freeList.empty())
        {
            OFUInt32 index = _freeList.back();
            _freeList.pop_back();
            _objects[index] = obj;
            return index + 1;
        }
        else
        {
            _objects.push_back(obj);
            return (OFUInt32)_objects.size();
        }
    }

    OFUInt32 getObjectsSize()
    {
        return (OFUInt32)_objects.size();
    }

    const OFUInt32 getObjectsSize() const
    {
        return (OFUInt32)_objects.size();
    }

    void remove(OFUInt32 handle)
    {
        OF_ASSERT(handle > 0 && handle <= _objects.size());
        _objects[handle - 1] = OF_NULL; // Set null.
        _freeList.push_back(handle - 1);
    }

    T &getRef(OFUInt32 handle)
    {
        OF_ASSERT(handle > 0 && handle <= _objects.size());
        return _objects[handle - 1];
    }

    const T &getRef(OFUInt32 handle) const
    {
        OF_ASSERT(handle > 0 && handle <= _objects.size());
        return _objects[handle - 1];
    }

private:
    std::vector<T> _objects;
    std::vector<OFUInt32> _freeList;
};

class RefObjPrivate;
class OF_EFFECT_API RefObj
{
    OF_DECLARE_PRIVATE(RefObj)
public:
    RefObj();
    virtual ~RefObj();

    int incRef() const;
    virtual void decRef();
    int getRefCount() const;
};

class ResObjPrivate;
class OF_EFFECT_API ResObj : public RefObj
{
    OF_DECLARE_PRIVATE(ResObj)
public:
    ResObj();
    virtual ~ResObj();

    virtual void decRef();
    const char *filePath() const;
    void setFilePath(const char *szFilePath);
};

} // namespace OrangeFilter

