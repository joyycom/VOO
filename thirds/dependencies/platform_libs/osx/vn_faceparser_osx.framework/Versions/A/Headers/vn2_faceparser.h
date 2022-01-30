//
// vn2_faceparser.h
//

#ifndef vn2_faceparser_h
#define vn2_faceparser_h

#include "vn2_core.h"
#include "vn2_common.h"

#ifdef __cplusplus
extern "C" {
#endif

    /// VN2_Create_FaceParser
    /// @param handle Handle of FaceParser context
    /// @param argc Argument counter
    /// @param argv Argument vector
    VN2_VENUS_API VN2_Result VN2_Create_FaceParser(VN2Handle* handle, const int argc, const void* argv[]);

    /// VN2_Destroy_FaceParser
    /// @param handle Handle of FaceParser context
    VN2_VENUS_API VN2_Result VN2_Destroy_FaceParser(VN2Handle* handle);

    /// VN2_Apply_FaceParser_CPU
    /// @param handle Handle of FaceParser context
    /// @param in_image Input VN2_Image pointer
    /// @param face_data Input VN2_FaceFrameDataArr pointer
    /// @param output Output VN2_ImageArr pointer
    VN2_VENUS_API VN2_Result VN2_Apply_FaceParser_CPU(VN2Handle handle, const void* in_image, const void* face_data, void* output);

#ifdef VN_BACKEND_METAL

    /// VN2_Apply_FaceParser_GPU
    /// @param handle Handle of FaceParser context
    /// @param in_image Input VN2_Image pointer
    /// @param face_data Input VN2_FaceFrameDataArr pointer
    /// @param output Output VN2_ImageArr pointer
    VN2_VENUS_API VN2_Result VN2_Apply_FaceParser_GPU(VN2Handle handle, const void* in_image, const void* face_data, void* output);
#endif
    /// VN_Set_FaceParser
    /// @param handle Handle of FaceParser context
    /// @param name Name of attribute
    /// @param value Value of attribute
    VN2_VENUS_API VN2_Result VN2_Set_FaceParser(VN2Handle handle, const char* name, const void* value);

    /// VN2_Get_FaceParser
    /// @param handle Handle of FaceParser context
    /// @param name Name of attribute
    /// @param value Value of attribute
    VN2_VENUS_API VN2_Result VN2_Get_FaceParser(VN2Handle handle, const char* name, void* value);


//#if    VN_BUILD_CART_FACE_MAKER
/// VN2_Create_CartFaceMaker
/// @param handle Handle of CartFaceMaker context
/// @param argc Argument counter
/// @param argv Argument vector
VN2_VENUS_API VN2_Result VN2_Create_CartFaceMaker(VN2Handle* handle, const int argc, const void* argv[]);

/// VN2_Destroy_CartFaceMaker
/// @param handle Handle of CartFaceMaker context
VN2_VENUS_API VN2_Result VN2_Destroy_CartFaceMaker(VN2Handle* handle);

/// VN2_Apply_CartFaceMaker_CPU
/// @param handle Handle of CartFaceMaker context
/// @param in_image Input VN2_Image pointer
/// @param face_data Input VN2_FaceFrameDataArr pointer
/// @param output Output VN2_ImageArr pointer
VN2_VENUS_API VN2_Result VN2_Apply_CartFaceMaker_CPU(VN2Handle handle, const void* in_image, const void* face_data, void* output);

#ifdef VN_BACKEND_METAL
/// VN2_Apply_CartFaceMaker_GPU
/// @param handle Handle of CartFaceMaker context
/// @param in_image Input VN2_Image pointer
/// @param face_data Input VN2_FaceFrameDataArr pointer
/// @param output Output VN2_ImageArr pointer
VN2_VENUS_API VN2_Result VN2_Apply_CartFaceMaker_GPU(VN2Handle handle, const void* in_image, const void* face_data, void* output);
#endif

/// VN_Set_CartFaceMaker
/// @param handle Handle of CartFaceMaker context
/// @param name Name of attribute
/// @param value Value of attribute
VN2_VENUS_API VN2_Result VN2_Set_CartFaceMaker(VN2Handle handle, const char* name, const void* value);

/// VN2_Get_CartFaceMaker
/// @param handle Handle of CartFaceMaker context
/// @param name Name of attribute
/// @param value Value of attribute
VN2_VENUS_API VN2_Result VN2_Get_CartFaceMaker(VN2Handle handle, const char* name, void* value);
//#endif

#ifdef __cplusplus
}
#endif


#endif
