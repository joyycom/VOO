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

#pragma once

#if defined(OF_IOS)
    #include "TargetConditionals.h"
    #ifdef OF_USE_GLES3
        #include <OpenGLES/ES3/gl.h>
        #include <OpenGLES/ES3/glext.h>
    #else
        #include <OpenGLES/ES2/gl.h>
        #include <OpenGLES/ES2/glext.h>
    #endif // OF_USE_GLES3
#elif defined(OF_ANDROID)
    #ifdef OF_USE_GLES31
        #include <GLES3/gl31.h>
    #elif defined(OF_USE_GLES3)
        #include <GLES3/gl3.h>
        #include <GLES3/gl3ext.h>
    #else
        #include "platform/android/gl3stub.h"
        #include <GLES2/gl2ext.h>
    #endif
#elif defined(OF_MAC)
    #ifdef USE_GLES_LIBRARY
        #ifdef OF_USE_GLES31
            #include "GLES3/gl31.h"
        #elif defined(OF_USE_GLES3)
            #include "GLES3/gl3.h"
        #else
            #include "GLES2/gl2.h"
            #include "GLES2/gl2ext.h"
        #endif // OF_USE_GLES3
    #else
        #include "TargetConditionals.h"
//        #include "GL/glew.h"
        #ifdef OF_USE_GLES3
            #include "OpenGL/gl3.h"
            #include "OpenGL/gl3ext.h"
        #else
            #include "OpenGL/gl.h"
            #include "OpenGL/glext.h"
        #endif

        #include "GLUT/GLUT.h"
    #endif
#elif defined(OF_WIN32) || defined(_WIN32) || defined(WIN32)
    #ifdef USE_GLES_LIBRARY
        #ifdef OF_USE_GLES31
            #include "GLES3/gl31.h"
        #elif defined(OF_USE_GLES3)
            #include "GLES3/gl3.h"
        #else
            #include "GLES2/gl2.h"
            #include "GLES2/gl2ext.h"
		    #ifdef __cplusplus
			    extern "C" {
		    #endif
			    GL_APICALL void GL_APIENTRY glVertexAttribDivisor(GLuint index, GLuint divisor);
			    GL_APICALL void GL_APIENTRY glDrawElementsInstanced(GLenum mode, GLsizei count, GLenum type, const void *indices, GLsizei instancecount);
		    #ifdef __cplusplus
			    }
		    #endif
        #endif // OF_USE_GLES3
    #else
        #include "GL/glew.h"
    #endif
#elif defined(OF_LINUX)
    #include <GL/osmesa.h>
    #include <GL/glcorearb.h>
#elif defined(OF_WASM)
    #ifdef OF_USE_GLES31
        #include <GLES3/gl31.h>
    #elif defined(OF_USE_GLES3)
        #include <GLES3/gl3.h>
        #include <GLES3/gl2ext.h>
    #else
        #include <GLES2/gl2.h>
        #include <GLES2/gl2ext.h>

        #ifndef GL_COLOR_ATTACHMENT1
        #define GL_COLOR_ATTACHMENT1 0x8CE1
        #endif
        #ifndef GL_COLOR_ATTACHMENT2
        #define GL_COLOR_ATTACHMENT2 0x8CE2
        #endif
        #ifndef GL_COLOR_ATTACHMENT3
        #define GL_COLOR_ATTACHMENT3 0x8CE3
        #endif
        #ifndef GL_DEPTH_COMPONENT24
        #define GL_DEPTH_COMPONENT24 0x81A6
        #endif
        #ifndef GL_READ_FRAMEBUFFER
        #define GL_READ_FRAMEBUFFER 0x8CA8
        #endif
        #ifndef GL_DRAW_FRAMEBUFFER
        #define GL_DRAW_FRAMEBUFFER 0x8CA9
        #endif
        #ifndef GL_RGBA16F
        #define GL_RGBA16F 0x881A
        #endif
        #ifndef GL_HALF_FLOAT_ARB
        #define GL_HALF_FLOAT_ARB 0x140B
        #endif
        #ifndef GL_SRGB8_ALPHA8
        #define GL_SRGB8_ALPHA8 0x8C43
        #endif
        #ifndef GL_SRGB8
        #define GL_SRGB8 0x8C41
        #endif
        #ifndef GL_TEXTURE_WRAP_R
        #define GL_TEXTURE_WRAP_R 0x8072
        #endif
        #ifndef GL_HALF_FLOAT
        #define GL_HALF_FLOAT 0x140B
        #endif
        #ifndef GL_RED
        #define GL_RED 0x1903
        #endif
        #ifndef GL_GREEN
        #define GL_GREEN 0x1904
        #endif
        #ifndef GL_BLUE
        #define GL_BLUE 0x1905
        #endif
        #ifndef GL_PIXEL_PACK_BUFFER
        #define GL_PIXEL_PACK_BUFFER 0x88EB
        #endif
        #ifndef GL_STREAM_READ
        #define GL_STREAM_READ 0x88E1
        #endif

        #define glVertexAttribDivisor glVertexAttribDivisorNV
        #define glDrawElementsInstanced glDrawElementsInstancedNV
        #define glDrawArraysInstanced glDrawArraysInstancedNV
        #define glDrawBuffers glDrawBuffersNV
        #define glMapBufferRange glMapBufferRangeEXT
        #define glUnmapBuffer glUnmapBufferOES

    #endif
#endif
