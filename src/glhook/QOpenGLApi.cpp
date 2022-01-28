#include <QtGui/QOpenGLFunctions_3_3_Core>
#include "QOpenGLApi.h"

static QOpenGLFunctions_3_3_Core *s_glCore;

void GLEW_setupOpenGLFunctions(QOpenGLFunctions_3_3_Core *core) {
    s_glCore = core;
}

// clang-format off
void GLAPIENTRY qt_glViewport(GLint x, GLint y, GLsizei width, GLsizei height) {
    s_glCore->glViewport(x, y, width, height);
}

void GLAPIENTRY qt_glDepthRange(GLdouble nearVal, GLdouble farVal) {
    s_glCore->glDepthRange(nearVal, farVal);
}

GLboolean GLAPIENTRY qt_glIsEnabled(GLenum cap) {
    return s_glCore->glIsEnabled(cap);
}

void GLAPIENTRY qt_glGetTexLevelParameteriv(GLenum target, GLint level, GLenum pname, GLint *params) {
    s_glCore->glGetTexLevelParameteriv(target, level, pname, params);
}

void GLAPIENTRY qt_glGetTexLevelParameterfv(GLenum target, GLint level, GLenum pname, GLfloat *params) {
    s_glCore->glGetTexLevelParameterfv(target, level, pname, params);
}

void GLAPIENTRY qt_glGetTexParameteriv(GLenum target, GLenum pname, GLint *params) {
    s_glCore->glGetTexParameteriv(target, pname, params);
}

void GLAPIENTRY qt_glGetTexParameterfv(GLenum target, GLenum pname, GLfloat *params) {
    s_glCore->glGetTexParameterfv(target, pname, params);
}

void GLAPIENTRY qt_glGetTexImage(GLenum target, GLint level, GLenum format, GLenum type, GLvoid *pixels) {
    s_glCore->glGetTexImage(target, level, format, type, pixels);
}

const GLubyte * GLAPIENTRY qt_glGetString(GLenum name) {
    return s_glCore->glGetString(name);
}

void GLAPIENTRY qt_glGetIntegerv(GLenum pname, GLint *params) {
    s_glCore->glGetIntegerv(pname, params);
}

void GLAPIENTRY qt_glGetFloatv(GLenum pname, GLfloat *params) {
    s_glCore->glGetFloatv(pname, params);
}

GLenum GLAPIENTRY qt_glGetError() {
    return s_glCore->glGetError();
}

void GLAPIENTRY qt_glGetDoublev(GLenum pname, GLdouble *params) {
    s_glCore->glGetDoublev(pname, params);
}

void GLAPIENTRY qt_glGetBooleanv(GLenum pname, GLboolean *params) {
    s_glCore->glGetBooleanv(pname, params);
}

void GLAPIENTRY qt_glReadPixels(GLint x, GLint y, GLsizei width, GLsizei height, GLenum format, GLenum type, GLvoid *pixels) {
    s_glCore->glReadPixels(x, y, width, height, format, type, pixels);
}

void GLAPIENTRY qt_glReadBuffer(GLenum mode) {
    s_glCore->glReadBuffer(mode);
}

void GLAPIENTRY qt_glPixelStorei(GLenum pname, GLint param) {
    s_glCore->glPixelStorei(pname, param);
}

void GLAPIENTRY qt_glPixelStoref(GLenum pname, GLfloat param) {
    s_glCore->glPixelStoref(pname, param);
}

void GLAPIENTRY qt_glDepthFunc(GLenum func) {
    s_glCore->glDepthFunc(func);
}

void GLAPIENTRY qt_glStencilOp(GLenum fail, GLenum zfail, GLenum zpass) {
    s_glCore->glStencilOp(fail, zfail, zpass);
}

void GLAPIENTRY qt_glStencilFunc(GLenum func, GLint ref, GLuint mask) {
    s_glCore->glStencilFunc(func, ref, mask);
}

void GLAPIENTRY qt_glLogicOp(GLenum opcode) {
    s_glCore->glLogicOp(opcode);
}

void GLAPIENTRY qt_glBlendFunc(GLenum sfactor, GLenum dfactor) {
    s_glCore->glBlendFunc(sfactor, dfactor);
}

void GLAPIENTRY qt_glFlush() {
    s_glCore->glFlush();
}

void GLAPIENTRY qt_glFinish() {
    s_glCore->glFinish();
}

void GLAPIENTRY qt_glEnable(GLenum cap) {
    s_glCore->glEnable(cap);
}

void GLAPIENTRY qt_glDisable(GLenum cap) {
    s_glCore->glDisable(cap);
}

void GLAPIENTRY qt_glDepthMask(GLboolean flag) {
    s_glCore->glDepthMask(flag);
}

void GLAPIENTRY qt_glColorMask(GLboolean red, GLboolean green, GLboolean blue, GLboolean alpha) {
    s_glCore->glColorMask(red, green, blue, alpha);
}

void GLAPIENTRY qt_glStencilMask(GLuint mask) {
    s_glCore->glStencilMask(mask);
}

void GLAPIENTRY qt_glClearDepth(GLdouble depth) {
    s_glCore->glClearDepth(depth);
}

void GLAPIENTRY qt_glClearStencil(GLint s) {
    s_glCore->glClearStencil(s);
}

void GLAPIENTRY qt_glClearColor(GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha) {
    s_glCore->glClearColor(red, green, blue, alpha);
}

void GLAPIENTRY qt_glClear(GLbitfield mask) {
    s_glCore->glClear(mask);
}

void GLAPIENTRY qt_glDrawBuffer(GLenum mode) {
    s_glCore->glDrawBuffer(mode);
}

void GLAPIENTRY qt_glTexImage2D(GLenum target, GLint level, GLint internalformat, GLsizei width, GLsizei height, GLint border, GLenum format, GLenum type, const GLvoid *pixels) {
    s_glCore->glTexImage2D(target, level, internalformat, width, height, border, format, type, pixels);
}

void GLAPIENTRY qt_glTexImage1D(GLenum target, GLint level, GLint internalformat, GLsizei width, GLint border, GLenum format, GLenum type, const GLvoid *pixels) {
    s_glCore->glTexImage1D(target, level, internalformat, width, border, format, type, pixels);
}

void GLAPIENTRY qt_glTexParameteriv(GLenum target, GLenum pname, const GLint *params) {
    s_glCore->glTexParameteriv(target, pname, params);
}

void GLAPIENTRY qt_glTexParameteri(GLenum target, GLenum pname, GLint param) {
    s_glCore->glTexParameteri(target, pname, param);
}

void GLAPIENTRY qt_glTexParameterfv(GLenum target, GLenum pname, const GLfloat *params) {
    s_glCore->glTexParameterfv(target, pname, params);
}

void GLAPIENTRY qt_glTexParameterf(GLenum target, GLenum pname, GLfloat param) {
    s_glCore->glTexParameterf(target, pname, param);
}

void GLAPIENTRY qt_glScissor(GLint x, GLint y, GLsizei width, GLsizei height) {
    s_glCore->glScissor(x, y, width, height);
}

void GLAPIENTRY qt_glPolygonMode(GLenum face, GLenum mode) {
    s_glCore->glPolygonMode(face, mode);
}

void GLAPIENTRY qt_glPointSize(GLfloat size) {
    s_glCore->glPointSize(size);
}

void GLAPIENTRY qt_glLineWidth(GLfloat width) {
    s_glCore->glLineWidth(width);
}

void GLAPIENTRY qt_glHint(GLenum target, GLenum mode) {
    s_glCore->glHint(target, mode);
}

void GLAPIENTRY qt_glFrontFace(GLenum mode) {
    s_glCore->glFrontFace(mode);
}

void GLAPIENTRY qt_glCullFace(GLenum mode) {
    s_glCore->glCullFace(mode);
}

void GLAPIENTRY qt_glIndexubv(const GLubyte *c) {
    s_glCore->glIndexubv(c);
}

void GLAPIENTRY qt_glIndexub(GLubyte c) {
    s_glCore->glIndexub(c);
}

GLboolean GLAPIENTRY qt_glIsTexture(GLuint texture) {
    return s_glCore->glIsTexture(texture);
}

void GLAPIENTRY qt_glGenTextures(GLsizei n, GLuint *textures) {
    s_glCore->glGenTextures(n, textures);
}

void GLAPIENTRY qt_glDeleteTextures(GLsizei n, const GLuint *textures) {
    s_glCore->glDeleteTextures(n, textures);
}

void GLAPIENTRY qt_glBindTexture(GLenum target, GLuint texture) {
    s_glCore->glBindTexture(target, texture);
}

void GLAPIENTRY qt_glTexSubImage2D(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLenum type, const GLvoid *pixels) {
    s_glCore->glTexSubImage2D(target, level, xoffset, yoffset, width, height, format, type, pixels);
}

void GLAPIENTRY qt_glTexSubImage1D(GLenum target, GLint level, GLint xoffset, GLsizei width, GLenum format, GLenum type, const GLvoid *pixels) {
    s_glCore->glTexSubImage1D(target, level, xoffset, width, format, type, pixels);
}

void GLAPIENTRY qt_glCopyTexSubImage2D(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint x, GLint y, GLsizei width, GLsizei height) {
    s_glCore->glCopyTexSubImage2D(target, level, xoffset, yoffset, x, y, width, height);
}

void GLAPIENTRY qt_glCopyTexSubImage1D(GLenum target, GLint level, GLint xoffset, GLint x, GLint y, GLsizei width) {
    s_glCore->glCopyTexSubImage1D(target, level, xoffset, x, y, width);
}

void GLAPIENTRY qt_glCopyTexImage2D(GLenum target, GLint level, GLenum internalformat, GLint x, GLint y, GLsizei width, GLsizei height, GLint border) {
    s_glCore->glCopyTexImage2D(target, level, internalformat, x, y, width, height, border);
}

void GLAPIENTRY qt_glCopyTexImage1D(GLenum target, GLint level, GLenum internalformat, GLint x, GLint y, GLsizei width, GLint border) {
    s_glCore->glCopyTexImage1D(target, level, internalformat, x, y, width, border);
}

void GLAPIENTRY qt_glPolygonOffset(GLfloat factor, GLfloat units) {
    s_glCore->glPolygonOffset(factor, units);
}

void GLAPIENTRY qt_glGetPointerv(GLenum pname, GLvoid* *params) {
    s_glCore->glGetPointerv(pname, params);
}

void GLAPIENTRY qt_glDrawElements(GLenum mode, GLsizei count, GLenum type, const GLvoid *indices) {
    s_glCore->glDrawElements(mode, count, type, indices);
}

void GLAPIENTRY qt_glDrawArrays(GLenum mode, GLint first, GLsizei count) {
    s_glCore->glDrawArrays(mode, first, count);
}

void GLAPIENTRY qt_glCopyTexSubImage3D(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLint x, GLint y, GLsizei width, GLsizei height) {
    s_glCore->glCopyTexSubImage3D(target, level, xoffset, yoffset, zoffset, x, y, width, height);
}

void GLAPIENTRY qt_glTexSubImage3D(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLenum format, GLenum type, const GLvoid *pixels) {
    s_glCore->glTexSubImage3D(target, level, xoffset, yoffset, zoffset, width, height, depth, format, type, pixels);
}

void GLAPIENTRY qt_glTexImage3D(GLenum target, GLint level, GLint internalformat, GLsizei width, GLsizei height, GLsizei depth, GLint border, GLenum format, GLenum type, const GLvoid *pixels) {
    s_glCore->glTexImage3D(target, level, internalformat, width, height, depth, border, format, type, pixels);
}

void GLAPIENTRY qt_glDrawRangeElements(GLenum mode, GLuint start, GLuint end, GLsizei count, GLenum type, const GLvoid *indices) {
    s_glCore->glDrawRangeElements(mode, start, end, count, type, indices);
}

void GLAPIENTRY qt_glBlendEquation(GLenum mode) {
    s_glCore->glBlendEquation(mode);
}

void GLAPIENTRY qt_glBlendColor(GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha) {
    s_glCore->glBlendColor(red, green, blue, alpha);
}

void GLAPIENTRY qt_glGetCompressedTexImage(GLenum target, GLint level, GLvoid *img) {
    s_glCore->glGetCompressedTexImage(target, level, img);
}

void GLAPIENTRY qt_glCompressedTexSubImage1D(GLenum target, GLint level, GLint xoffset, GLsizei width, GLenum format, GLsizei imageSize, const GLvoid *data) {
    s_glCore->glCompressedTexSubImage1D(target, level, xoffset, width, format, imageSize, data);
}

void GLAPIENTRY qt_glCompressedTexSubImage2D(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLsizei imageSize, const GLvoid *data) {
    s_glCore->glCompressedTexSubImage2D(target, level, xoffset, yoffset, width, height, format, imageSize, data);
}

void GLAPIENTRY qt_glCompressedTexSubImage3D(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLenum format, GLsizei imageSize, const GLvoid *data) {
    s_glCore->glCompressedTexSubImage3D(target, level, xoffset, yoffset, zoffset, width, height, depth, format, imageSize, data);
}

void GLAPIENTRY qt_glCompressedTexImage1D(GLenum target, GLint level, GLenum internalformat, GLsizei width, GLint border, GLsizei imageSize, const GLvoid *data) {
    s_glCore->glCompressedTexImage1D(target, level, internalformat, width, border, imageSize, data);
}

void GLAPIENTRY qt_glCompressedTexImage2D(GLenum target, GLint level, GLenum internalformat, GLsizei width, GLsizei height, GLint border, GLsizei imageSize, const GLvoid *data) {
    s_glCore->glCompressedTexImage2D(target, level, internalformat, width, height, border, imageSize, data);
}

void GLAPIENTRY qt_glCompressedTexImage3D(GLenum target, GLint level, GLenum internalformat, GLsizei width, GLsizei height, GLsizei depth, GLint border, GLsizei imageSize, const GLvoid *data) {
    s_glCore->glCompressedTexImage3D(target, level, internalformat, width, height, depth, border, imageSize, data);
}

void GLAPIENTRY qt_glSampleCoverage(GLfloat value, GLboolean invert) {
    s_glCore->glSampleCoverage(value, invert);
}

void GLAPIENTRY qt_glActiveTexture(GLenum texture) {
    s_glCore->glActiveTexture(texture);
}

void GLAPIENTRY qt_glPointParameteriv(GLenum pname, const GLint *params) {
    s_glCore->glPointParameteriv(pname, params);
}

void GLAPIENTRY qt_glPointParameteri(GLenum pname, GLint param) {
    s_glCore->glPointParameteri(pname, param);
}

void GLAPIENTRY qt_glPointParameterfv(GLenum pname, const GLfloat *params) {
    s_glCore->glPointParameterfv(pname, params);
}

void GLAPIENTRY qt_glPointParameterf(GLenum pname, GLfloat param) {
    s_glCore->glPointParameterf(pname, param);
}

void GLAPIENTRY qt_glMultiDrawElements(GLenum mode, const GLsizei *count, GLenum type, const GLvoid* const *indices, GLsizei drawcount) {
    s_glCore->glMultiDrawElements(mode, count, type, indices, drawcount);
}

void GLAPIENTRY qt_glMultiDrawArrays(GLenum mode, const GLint *first, const GLsizei *count, GLsizei drawcount) {
    s_glCore->glMultiDrawArrays(mode, first, count, drawcount);
}

void GLAPIENTRY qt_glBlendFuncSeparate(GLenum sfactorRGB, GLenum dfactorRGB, GLenum sfactorAlpha, GLenum dfactorAlpha) {
    s_glCore->glBlendFuncSeparate(sfactorRGB, dfactorRGB, sfactorAlpha, dfactorAlpha);
}

void GLAPIENTRY qt_glGetBufferPointerv(GLenum target, GLenum pname, GLvoid* *params) {
    s_glCore->glGetBufferPointerv(target, pname, params);
}

void GLAPIENTRY qt_glGetBufferParameteriv(GLenum target, GLenum pname, GLint *params) {
    s_glCore->glGetBufferParameteriv(target, pname, params);
}

GLboolean GLAPIENTRY qt_glUnmapBuffer(GLenum target) {
    return s_glCore->glUnmapBuffer(target);
}

GLvoid* GLAPIENTRY qt_glMapBuffer(GLenum target, GLenum access) {
    return s_glCore->glMapBuffer(target, access);
}

void GLAPIENTRY qt_glGetBufferSubData(GLenum target, GLintptr offset, GLsizeiptr size, GLvoid *data) {
    s_glCore->glGetBufferSubData(target, offset, size, data);
}

void GLAPIENTRY qt_glBufferSubData(GLenum target, GLintptr offset, GLsizeiptr size, const GLvoid *data) {
    s_glCore->glBufferSubData(target, offset, size, data);
}

void GLAPIENTRY qt_glBufferData(GLenum target, GLsizeiptr size, const GLvoid *data, GLenum usage) {
    s_glCore->glBufferData(target, size, data, usage);
}

GLboolean GLAPIENTRY qt_glIsBuffer(GLuint buffer) {
    return s_glCore->glIsBuffer(buffer);
}

void GLAPIENTRY qt_glGenBuffers(GLsizei n, GLuint *buffers) {
    s_glCore->glGenBuffers(n, buffers);
}

void GLAPIENTRY qt_glDeleteBuffers(GLsizei n, const GLuint *buffers) {
    s_glCore->glDeleteBuffers(n, buffers);
}

void GLAPIENTRY qt_glBindBuffer(GLenum target, GLuint buffer) {
    s_glCore->glBindBuffer(target, buffer);
}

void GLAPIENTRY qt_glGetQueryObjectuiv(GLuint id, GLenum pname, GLuint *params) {
    s_glCore->glGetQueryObjectuiv(id, pname, params);
}

void GLAPIENTRY qt_glGetQueryObjectiv(GLuint id, GLenum pname, GLint *params) {
    s_glCore->glGetQueryObjectiv(id, pname, params);
}

void GLAPIENTRY qt_glGetQueryiv(GLenum target, GLenum pname, GLint *params) {
    s_glCore->glGetQueryiv(target, pname, params);
}

void GLAPIENTRY qt_glEndQuery(GLenum target) {
    s_glCore->glEndQuery(target);
}

void GLAPIENTRY qt_glBeginQuery(GLenum target, GLuint id) {
    s_glCore->glBeginQuery(target, id);
}

GLboolean GLAPIENTRY qt_glIsQuery(GLuint id) {
    return s_glCore->glIsQuery(id);
}

void GLAPIENTRY qt_glDeleteQueries(GLsizei n, const GLuint *ids) {
    s_glCore->glDeleteQueries(n, ids);
}

void GLAPIENTRY qt_glGenQueries(GLsizei n, GLuint *ids) {
    s_glCore->glGenQueries(n, ids);
}

void GLAPIENTRY qt_glVertexAttribPointer(GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const GLvoid *pointer) {
    s_glCore->glVertexAttribPointer(index, size, type, normalized, stride, pointer);
}

void GLAPIENTRY qt_glValidateProgram(GLuint program) {
    s_glCore->glValidateProgram(program);
}

void GLAPIENTRY qt_glUniformMatrix4fv(GLint location, GLsizei count, GLboolean transpose, const GLfloat *value) {
    s_glCore->glUniformMatrix4fv(location, count, transpose, value);
}

void GLAPIENTRY qt_glUniformMatrix3fv(GLint location, GLsizei count, GLboolean transpose, const GLfloat *value) {
    s_glCore->glUniformMatrix3fv(location, count, transpose, value);
}

void GLAPIENTRY qt_glUniformMatrix2fv(GLint location, GLsizei count, GLboolean transpose, const GLfloat *value) {
    s_glCore->glUniformMatrix2fv(location, count, transpose, value);
}

void GLAPIENTRY qt_glUniform4iv(GLint location, GLsizei count, const GLint *value) {
    s_glCore->glUniform4iv(location, count, value);
}

void GLAPIENTRY qt_glUniform3iv(GLint location, GLsizei count, const GLint *value) {
    s_glCore->glUniform3iv(location, count, value);
}

void GLAPIENTRY qt_glUniform2iv(GLint location, GLsizei count, const GLint *value) {
    s_glCore->glUniform2iv(location, count, value);
}

void GLAPIENTRY qt_glUniform1iv(GLint location, GLsizei count, const GLint *value) {
    s_glCore->glUniform1iv(location, count, value);
}

void GLAPIENTRY qt_glUniform4fv(GLint location, GLsizei count, const GLfloat *value) {
    s_glCore->glUniform4fv(location, count, value);
}

void GLAPIENTRY qt_glUniform3fv(GLint location, GLsizei count, const GLfloat *value) {
    s_glCore->glUniform3fv(location, count, value);
}

void GLAPIENTRY qt_glUniform2fv(GLint location, GLsizei count, const GLfloat *value) {
    s_glCore->glUniform2fv(location, count, value);
}

void GLAPIENTRY qt_glUniform1fv(GLint location, GLsizei count, const GLfloat *value) {
    s_glCore->glUniform1fv(location, count, value);
}

void GLAPIENTRY qt_glUniform4i(GLint location, GLint v0, GLint v1, GLint v2, GLint v3) {
    s_glCore->glUniform4i(location, v0, v1, v2, v3);
}

void GLAPIENTRY qt_glUniform3i(GLint location, GLint v0, GLint v1, GLint v2) {
    s_glCore->glUniform3i(location, v0, v1, v2);
}

void GLAPIENTRY qt_glUniform2i(GLint location, GLint v0, GLint v1) {
    s_glCore->glUniform2i(location, v0, v1);
}

void GLAPIENTRY qt_glUniform1i(GLint location, GLint v0) {
    s_glCore->glUniform1i(location, v0);
}

void GLAPIENTRY qt_glUniform4f(GLint location, GLfloat v0, GLfloat v1, GLfloat v2, GLfloat v3) {
    s_glCore->glUniform4f(location, v0, v1, v2, v3);
}

void GLAPIENTRY qt_glUniform3f(GLint location, GLfloat v0, GLfloat v1, GLfloat v2) {
    s_glCore->glUniform3f(location, v0, v1, v2);
}

void GLAPIENTRY qt_glUniform2f(GLint location, GLfloat v0, GLfloat v1) {
    s_glCore->glUniform2f(location, v0, v1);
}

void GLAPIENTRY qt_glUniform1f(GLint location, GLfloat v0) {
    s_glCore->glUniform1f(location, v0);
}

void GLAPIENTRY qt_glUseProgram(GLuint program) {
    s_glCore->glUseProgram(program);
}

void GLAPIENTRY qt_glShaderSource(GLuint shader, GLsizei count, const GLchar* const *string, const GLint *length) {
    s_glCore->glShaderSource(shader, count, string, length);
}

void GLAPIENTRY qt_glLinkProgram(GLuint program) {
    s_glCore->glLinkProgram(program);
}

GLboolean GLAPIENTRY qt_glIsShader(GLuint shader) {
    return s_glCore->glIsShader(shader);
}

GLboolean GLAPIENTRY qt_glIsProgram(GLuint program) {
    return s_glCore->glIsProgram(program);
}

void GLAPIENTRY qt_glGetVertexAttribPointerv(GLuint index, GLenum pname, GLvoid* *pointer) {
    s_glCore->glGetVertexAttribPointerv(index, pname, pointer);
}

void GLAPIENTRY qt_glGetVertexAttribiv(GLuint index, GLenum pname, GLint *params) {
    s_glCore->glGetVertexAttribiv(index, pname, params);
}

void GLAPIENTRY qt_glGetVertexAttribfv(GLuint index, GLenum pname, GLfloat *params) {
    s_glCore->glGetVertexAttribfv(index, pname, params);
}

void GLAPIENTRY qt_glGetVertexAttribdv(GLuint index, GLenum pname, GLdouble *params) {
    s_glCore->glGetVertexAttribdv(index, pname, params);
}

void GLAPIENTRY qt_glGetUniformiv(GLuint program, GLint location, GLint *params) {
    s_glCore->glGetUniformiv(program, location, params);
}

void GLAPIENTRY qt_glGetUniformfv(GLuint program, GLint location, GLfloat *params) {
    s_glCore->glGetUniformfv(program, location, params);
}

GLint GLAPIENTRY qt_glGetUniformLocation(GLuint program, const GLchar *name) {
    return s_glCore->glGetUniformLocation(program, name);
}

void GLAPIENTRY qt_glGetShaderSource(GLuint shader, GLsizei bufSize, GLsizei *length, GLchar *source) {
    s_glCore->glGetShaderSource(shader, bufSize, length, source);
}

void GLAPIENTRY qt_glGetShaderInfoLog(GLuint shader, GLsizei bufSize, GLsizei *length, GLchar *infoLog) {
    s_glCore->glGetShaderInfoLog(shader, bufSize, length, infoLog);
}

void GLAPIENTRY qt_glGetShaderiv(GLuint shader, GLenum pname, GLint *params) {
    s_glCore->glGetShaderiv(shader, pname, params);
}

void GLAPIENTRY qt_glGetProgramInfoLog(GLuint program, GLsizei bufSize, GLsizei *length, GLchar *infoLog) {
    s_glCore->glGetProgramInfoLog(program, bufSize, length, infoLog);
}

void GLAPIENTRY qt_glGetProgramiv(GLuint program, GLenum pname, GLint *params) {
    s_glCore->glGetProgramiv(program, pname, params);
}

GLint GLAPIENTRY qt_glGetAttribLocation(GLuint program, const GLchar *name) {
    return s_glCore->glGetAttribLocation(program, name);
}

void GLAPIENTRY qt_glGetAttachedShaders(GLuint program, GLsizei maxCount, GLsizei *count, GLuint *obj) {
    s_glCore->glGetAttachedShaders(program, maxCount, count, obj);
}

void GLAPIENTRY qt_glGetActiveUniform(GLuint program, GLuint index, GLsizei bufSize, GLsizei *length, GLint *size, GLenum *type, GLchar *name) {
    s_glCore->glGetActiveUniform(program, index, bufSize, length, size, type, name);
}

void GLAPIENTRY qt_glGetActiveAttrib(GLuint program, GLuint index, GLsizei bufSize, GLsizei *length, GLint *size, GLenum *type, GLchar *name) {
    s_glCore->glGetActiveAttrib(program, index, bufSize, length, size, type, name);
}

void GLAPIENTRY qt_glEnableVertexAttribArray(GLuint index) {
    s_glCore->glEnableVertexAttribArray(index);
}

void GLAPIENTRY qt_glDisableVertexAttribArray(GLuint index) {
    s_glCore->glDisableVertexAttribArray(index);
}

void GLAPIENTRY qt_glDetachShader(GLuint program, GLuint shader) {
    s_glCore->glDetachShader(program, shader);
}

void GLAPIENTRY qt_glDeleteShader(GLuint shader) {
    s_glCore->glDeleteShader(shader);
}

void GLAPIENTRY qt_glDeleteProgram(GLuint program) {
    s_glCore->glDeleteProgram(program);
}

GLuint GLAPIENTRY qt_glCreateShader(GLenum type) {
    return s_glCore->glCreateShader(type);
}

GLuint GLAPIENTRY qt_glCreateProgram() {
    return s_glCore->glCreateProgram();
}

void GLAPIENTRY qt_glCompileShader(GLuint shader) {
    s_glCore->glCompileShader(shader);
}

void GLAPIENTRY qt_glBindAttribLocation(GLuint program, GLuint index, const GLchar *name) {
    s_glCore->glBindAttribLocation(program, index, name);
}

void GLAPIENTRY qt_glAttachShader(GLuint program, GLuint shader) {
    s_glCore->glAttachShader(program, shader);
}

void GLAPIENTRY qt_glStencilMaskSeparate(GLenum face, GLuint mask) {
    s_glCore->glStencilMaskSeparate(face, mask);
}

void GLAPIENTRY qt_glStencilFuncSeparate(GLenum face, GLenum func, GLint ref, GLuint mask) {
    s_glCore->glStencilFuncSeparate(face, func, ref, mask);
}

void GLAPIENTRY qt_glStencilOpSeparate(GLenum face, GLenum sfail, GLenum dpfail, GLenum dppass) {
    s_glCore->glStencilOpSeparate(face, sfail, dpfail, dppass);
}

void GLAPIENTRY qt_glDrawBuffers(GLsizei n, const GLenum *bufs) {
    s_glCore->glDrawBuffers(n, bufs);
}

void GLAPIENTRY qt_glBlendEquationSeparate(GLenum modeRGB, GLenum modeAlpha) {
    s_glCore->glBlendEquationSeparate(modeRGB, modeAlpha);
}

void GLAPIENTRY qt_glUniformMatrix4x3fv(GLint location, GLsizei count, GLboolean transpose, const GLfloat *value) {
    s_glCore->glUniformMatrix4x3fv(location, count, transpose, value);
}

void GLAPIENTRY qt_glUniformMatrix3x4fv(GLint location, GLsizei count, GLboolean transpose, const GLfloat *value) {
    s_glCore->glUniformMatrix3x4fv(location, count, transpose, value);
}

void GLAPIENTRY qt_glUniformMatrix4x2fv(GLint location, GLsizei count, GLboolean transpose, const GLfloat *value) {
    s_glCore->glUniformMatrix4x2fv(location, count, transpose, value);
}

void GLAPIENTRY qt_glUniformMatrix2x4fv(GLint location, GLsizei count, GLboolean transpose, const GLfloat *value) {
    s_glCore->glUniformMatrix2x4fv(location, count, transpose, value);
}

void GLAPIENTRY qt_glUniformMatrix3x2fv(GLint location, GLsizei count, GLboolean transpose, const GLfloat *value) {
    s_glCore->glUniformMatrix3x2fv(location, count, transpose, value);
}

void GLAPIENTRY qt_glUniformMatrix2x3fv(GLint location, GLsizei count, GLboolean transpose, const GLfloat *value) {
    s_glCore->glUniformMatrix2x3fv(location, count, transpose, value);
}

GLboolean GLAPIENTRY qt_glIsVertexArray(GLuint array) {
    return s_glCore->glIsVertexArray(array);
}

void GLAPIENTRY qt_glGenVertexArrays(GLsizei n, GLuint *arrays) {
    s_glCore->glGenVertexArrays(n, arrays);
}

void GLAPIENTRY qt_glDeleteVertexArrays(GLsizei n, const GLuint *arrays) {
    s_glCore->glDeleteVertexArrays(n, arrays);
}

void GLAPIENTRY qt_glBindVertexArray(GLuint array) {
    s_glCore->glBindVertexArray(array);
}

void GLAPIENTRY qt_glFlushMappedBufferRange(GLenum target, GLintptr offset, GLsizeiptr length) {
    s_glCore->glFlushMappedBufferRange(target, offset, length);
}

GLvoid* GLAPIENTRY qt_glMapBufferRange(GLenum target, GLintptr offset, GLsizeiptr length, GLbitfield access) {
    return s_glCore->glMapBufferRange(target, offset, length, access);
}

void GLAPIENTRY qt_glFramebufferTextureLayer(GLenum target, GLenum attachment, GLuint texture, GLint level, GLint layer) {
    s_glCore->glFramebufferTextureLayer(target, attachment, texture, level, layer);
}

void GLAPIENTRY qt_glRenderbufferStorageMultisample(GLenum target, GLsizei samples, GLenum internalformat, GLsizei width, GLsizei height) {
    s_glCore->glRenderbufferStorageMultisample(target, samples, internalformat, width, height);
}

void GLAPIENTRY qt_glBlitFramebuffer(GLint srcX0, GLint srcY0, GLint srcX1, GLint srcY1, GLint dstX0, GLint dstY0, GLint dstX1, GLint dstY1, GLbitfield mask, GLenum filter) {
    s_glCore->glBlitFramebuffer(srcX0, srcY0, srcX1, srcY1, dstX0, dstY0, dstX1, dstY1, mask, filter);
}

void GLAPIENTRY qt_glGenerateMipmap(GLenum target) {
    s_glCore->glGenerateMipmap(target);
}

void GLAPIENTRY qt_glGetFramebufferAttachmentParameteriv(GLenum target, GLenum attachment, GLenum pname, GLint *params) {
    s_glCore->glGetFramebufferAttachmentParameteriv(target, attachment, pname, params);
}

void GLAPIENTRY qt_glFramebufferRenderbuffer(GLenum target, GLenum attachment, GLenum renderbuffertarget, GLuint renderbuffer) {
    s_glCore->glFramebufferRenderbuffer(target, attachment, renderbuffertarget, renderbuffer);
}

void GLAPIENTRY qt_glFramebufferTexture3D(GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level, GLint zoffset) {
    s_glCore->glFramebufferTexture3D(target, attachment, textarget, texture, level, zoffset);
}

void GLAPIENTRY qt_glFramebufferTexture2D(GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level) {
    s_glCore->glFramebufferTexture2D(target, attachment, textarget, texture, level);
}

void GLAPIENTRY qt_glFramebufferTexture1D(GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level) {
    s_glCore->glFramebufferTexture1D(target, attachment, textarget, texture, level);
}

GLenum GLAPIENTRY qt_glCheckFramebufferStatus(GLenum target) {
    return s_glCore->glCheckFramebufferStatus(target);
}

void GLAPIENTRY qt_glGenFramebuffers(GLsizei n, GLuint *framebuffers) {
    s_glCore->glGenFramebuffers(n, framebuffers);
}

void GLAPIENTRY qt_glDeleteFramebuffers(GLsizei n, const GLuint *framebuffers) {
    s_glCore->glDeleteFramebuffers(n, framebuffers);
}

void GLAPIENTRY qt_glBindFramebuffer(GLenum target, GLuint framebuffer) {
    s_glCore->glBindFramebuffer(target, framebuffer);
}

GLboolean GLAPIENTRY qt_glIsFramebuffer(GLuint framebuffer) {
    return s_glCore->glIsFramebuffer(framebuffer);
}

void GLAPIENTRY qt_glGetRenderbufferParameteriv(GLenum target, GLenum pname, GLint *params) {
    s_glCore->glGetRenderbufferParameteriv(target, pname, params);
}

void GLAPIENTRY qt_glRenderbufferStorage(GLenum target, GLenum internalformat, GLsizei width, GLsizei height) {
    s_glCore->glRenderbufferStorage(target, internalformat, width, height);
}

void GLAPIENTRY qt_glGenRenderbuffers(GLsizei n, GLuint *renderbuffers) {
    s_glCore->glGenRenderbuffers(n, renderbuffers);
}

void GLAPIENTRY qt_glDeleteRenderbuffers(GLsizei n, const GLuint *renderbuffers) {
    s_glCore->glDeleteRenderbuffers(n, renderbuffers);
}

void GLAPIENTRY qt_glBindRenderbuffer(GLenum target, GLuint renderbuffer) {
    s_glCore->glBindRenderbuffer(target, renderbuffer);
}

GLboolean GLAPIENTRY qt_glIsRenderbuffer(GLuint renderbuffer) {
    return s_glCore->glIsRenderbuffer(renderbuffer);
}

const GLubyte * GLAPIENTRY qt_glGetStringi(GLenum name, GLuint index) {
    return s_glCore->glGetStringi(name, index);
}

void GLAPIENTRY qt_glClearBufferfi(GLenum buffer, GLint drawbuffer, GLfloat depth, GLint stencil) {
    s_glCore->glClearBufferfi(buffer, drawbuffer, depth, stencil);
}

void GLAPIENTRY qt_glClearBufferfv(GLenum buffer, GLint drawbuffer, const GLfloat *value) {
    s_glCore->glClearBufferfv(buffer, drawbuffer, value);
}

void GLAPIENTRY qt_glClearBufferuiv(GLenum buffer, GLint drawbuffer, const GLuint *value) {
    s_glCore->glClearBufferuiv(buffer, drawbuffer, value);
}

void GLAPIENTRY qt_glClearBufferiv(GLenum buffer, GLint drawbuffer, const GLint *value) {
    s_glCore->glClearBufferiv(buffer, drawbuffer, value);
}

void GLAPIENTRY qt_glGetTexParameterIuiv(GLenum target, GLenum pname, GLuint *params) {
    s_glCore->glGetTexParameterIuiv(target, pname, params);
}

void GLAPIENTRY qt_glGetTexParameterIiv(GLenum target, GLenum pname, GLint *params) {
    s_glCore->glGetTexParameterIiv(target, pname, params);
}

void GLAPIENTRY qt_glTexParameterIuiv(GLenum target, GLenum pname, const GLuint *params) {
    s_glCore->glTexParameterIuiv(target, pname, params);
}

void GLAPIENTRY qt_glTexParameterIiv(GLenum target, GLenum pname, const GLint *params) {
    s_glCore->glTexParameterIiv(target, pname, params);
}

void GLAPIENTRY qt_glUniform4uiv(GLint location, GLsizei count, const GLuint *value) {
    s_glCore->glUniform4uiv(location, count, value);
}

void GLAPIENTRY qt_glUniform3uiv(GLint location, GLsizei count, const GLuint *value) {
    s_glCore->glUniform3uiv(location, count, value);
}

void GLAPIENTRY qt_glUniform2uiv(GLint location, GLsizei count, const GLuint *value) {
    s_glCore->glUniform2uiv(location, count, value);
}

void GLAPIENTRY qt_glUniform1uiv(GLint location, GLsizei count, const GLuint *value) {
    s_glCore->glUniform1uiv(location, count, value);
}

void GLAPIENTRY qt_glUniform4ui(GLint location, GLuint v0, GLuint v1, GLuint v2, GLuint v3) {
    s_glCore->glUniform4ui(location, v0, v1, v2, v3);
}

void GLAPIENTRY qt_glUniform3ui(GLint location, GLuint v0, GLuint v1, GLuint v2) {
    s_glCore->glUniform3ui(location, v0, v1, v2);
}

void GLAPIENTRY qt_glUniform2ui(GLint location, GLuint v0, GLuint v1) {
    s_glCore->glUniform2ui(location, v0, v1);
}

void GLAPIENTRY qt_glUniform1ui(GLint location, GLuint v0) {
    s_glCore->glUniform1ui(location, v0);
}

GLint GLAPIENTRY qt_glGetFragDataLocation(GLuint program, const GLchar *name) {
    return s_glCore->glGetFragDataLocation(program, name);
}

void GLAPIENTRY qt_glBindFragDataLocation(GLuint program, GLuint color, const GLchar *name) {
    s_glCore->glBindFragDataLocation(program, color, name);
}

void GLAPIENTRY qt_glGetUniformuiv(GLuint program, GLint location, GLuint *params) {
    s_glCore->glGetUniformuiv(program, location, params);
}

void GLAPIENTRY qt_glGetVertexAttribIuiv(GLuint index, GLenum pname, GLuint *params) {
    s_glCore->glGetVertexAttribIuiv(index, pname, params);
}

void GLAPIENTRY qt_glGetVertexAttribIiv(GLuint index, GLenum pname, GLint *params) {
    s_glCore->glGetVertexAttribIiv(index, pname, params);
}

void GLAPIENTRY qt_glVertexAttribIPointer(GLuint index, GLint size, GLenum type, GLsizei stride, const GLvoid *pointer) {
    s_glCore->glVertexAttribIPointer(index, size, type, stride, pointer);
}

void GLAPIENTRY qt_glEndConditionalRender() {
    s_glCore->glEndConditionalRender();
}

void GLAPIENTRY qt_glBeginConditionalRender(GLuint id, GLenum mode) {
    s_glCore->glBeginConditionalRender(id, mode);
}

void GLAPIENTRY qt_glClampColor(GLenum target, GLenum clamp) {
    s_glCore->glClampColor(target, clamp);
}

void GLAPIENTRY qt_glGetTransformFeedbackVarying(GLuint program, GLuint index, GLsizei bufSize, GLsizei *length, GLsizei *size, GLenum *type, GLchar *name) {
    s_glCore->glGetTransformFeedbackVarying(program, index, bufSize, length, size, type, name);
}

void GLAPIENTRY qt_glTransformFeedbackVaryings(GLuint program, GLsizei count, const GLchar* const *varyings, GLenum bufferMode) {
    s_glCore->glTransformFeedbackVaryings(program, count, varyings, bufferMode);
}

void GLAPIENTRY qt_glBindBufferBase(GLenum target, GLuint index, GLuint buffer) {
    s_glCore->glBindBufferBase(target, index, buffer);
}

void GLAPIENTRY qt_glBindBufferRange(GLenum target, GLuint index, GLuint buffer, GLintptr offset, GLsizeiptr size) {
    s_glCore->glBindBufferRange(target, index, buffer, offset, size);
}

void GLAPIENTRY qt_glEndTransformFeedback() {
    s_glCore->glEndTransformFeedback();
}

void GLAPIENTRY qt_glBeginTransformFeedback(GLenum primitiveMode) {
    s_glCore->glBeginTransformFeedback(primitiveMode);
}

GLboolean GLAPIENTRY qt_glIsEnabledi(GLenum target, GLuint index) {
    return s_glCore->glIsEnabledi(target, index);
}

void GLAPIENTRY qt_glDisablei(GLenum target, GLuint index) {
    s_glCore->glDisablei(target, index);
}

void GLAPIENTRY qt_glEnablei(GLenum target, GLuint index) {
    s_glCore->glEnablei(target, index);
}

void GLAPIENTRY qt_glGetIntegeri_v(GLenum target, GLuint index, GLint *data) {
    s_glCore->glGetIntegeri_v(target, index, data);
}

void GLAPIENTRY qt_glGetBooleani_v(GLenum target, GLuint index, GLboolean *data) {
    s_glCore->glGetBooleani_v(target, index, data);
}

void GLAPIENTRY qt_glColorMaski(GLuint index, GLboolean r, GLboolean g, GLboolean b, GLboolean a) {
    s_glCore->glColorMaski(index, r, g, b, a);
}

void GLAPIENTRY qt_glCopyBufferSubData(GLenum readTarget, GLenum writeTarget, GLintptr readOffset, GLintptr writeOffset, GLsizeiptr size) {
    s_glCore->glCopyBufferSubData(readTarget, writeTarget, readOffset, writeOffset, size);
}

void GLAPIENTRY qt_glUniformBlockBinding(GLuint program, GLuint uniformBlockIndex, GLuint uniformBlockBinding) {
    s_glCore->glUniformBlockBinding(program, uniformBlockIndex, uniformBlockBinding);
}

void GLAPIENTRY qt_glGetActiveUniformBlockName(GLuint program, GLuint uniformBlockIndex, GLsizei bufSize, GLsizei *length, GLchar *uniformBlockName) {
    s_glCore->glGetActiveUniformBlockName(program, uniformBlockIndex, bufSize, length, uniformBlockName);
}

void GLAPIENTRY qt_glGetActiveUniformBlockiv(GLuint program, GLuint uniformBlockIndex, GLenum pname, GLint *params) {
    s_glCore->glGetActiveUniformBlockiv(program, uniformBlockIndex, pname, params);
}

GLuint GLAPIENTRY qt_glGetUniformBlockIndex(GLuint program, const GLchar *uniformBlockName) {
    return s_glCore->glGetUniformBlockIndex(program, uniformBlockName);
}

void GLAPIENTRY qt_glGetActiveUniformName(GLuint program, GLuint uniformIndex, GLsizei bufSize, GLsizei *length, GLchar *uniformName) {
    s_glCore->glGetActiveUniformName(program, uniformIndex, bufSize, length, uniformName);
}

void GLAPIENTRY qt_glGetActiveUniformsiv(GLuint program, GLsizei uniformCount, const GLuint *uniformIndices, GLenum pname, GLint *params) {
    s_glCore->glGetActiveUniformsiv(program, uniformCount, uniformIndices, pname, params);
}

void GLAPIENTRY qt_glGetUniformIndices(GLuint program, GLsizei uniformCount, const GLchar* const *uniformNames, GLuint *uniformIndices) {
    s_glCore->glGetUniformIndices(program, uniformCount, uniformNames, uniformIndices);
}

void GLAPIENTRY qt_glPrimitiveRestartIndex(GLuint index) {
    s_glCore->glPrimitiveRestartIndex(index);
}

void GLAPIENTRY qt_glTexBuffer(GLenum target, GLenum internalformat, GLuint buffer) {
    s_glCore->glTexBuffer(target, internalformat, buffer);
}

void GLAPIENTRY qt_glDrawElementsInstanced(GLenum mode, GLsizei count, GLenum type, const GLvoid *indices, GLsizei instancecount) {
    s_glCore->glDrawElementsInstanced(mode, count, type, indices, instancecount);
}

void GLAPIENTRY qt_glDrawArraysInstanced(GLenum mode, GLint first, GLsizei count, GLsizei instancecount) {
    s_glCore->glDrawArraysInstanced(mode, first, count, instancecount);
}

void GLAPIENTRY qt_glSampleMaski(GLuint index, GLbitfield mask) {
    s_glCore->glSampleMaski(index, mask);
}

void GLAPIENTRY qt_glGetMultisamplefv(GLenum pname, GLuint index, GLfloat *val) {
    s_glCore->glGetMultisamplefv(pname, index, val);
}

void GLAPIENTRY qt_glTexImage3DMultisample(GLenum target, GLsizei samples, GLint internalformat, GLsizei width, GLsizei height, GLsizei depth, GLboolean fixedsamplelocations) {
    s_glCore->glTexImage3DMultisample(target, samples, internalformat, width, height, depth, fixedsamplelocations);
}

void GLAPIENTRY qt_glTexImage2DMultisample(GLenum target, GLsizei samples, GLint internalformat, GLsizei width, GLsizei height, GLboolean fixedsamplelocations) {
    s_glCore->glTexImage2DMultisample(target, samples, internalformat, width, height, fixedsamplelocations);
}

void GLAPIENTRY qt_glGetSynciv(GLsync sync, GLenum pname, GLsizei bufSize, GLsizei *length, GLint *values) {
    s_glCore->glGetSynciv(sync, pname, bufSize, length, values);
}

void GLAPIENTRY qt_glGetInteger64v(GLenum pname, GLint64 *params) {
    s_glCore->glGetInteger64v(pname, params);
}

void GLAPIENTRY qt_glWaitSync(GLsync sync, GLbitfield flags, GLuint64 timeout) {
    s_glCore->glWaitSync(sync, flags, timeout);
}

GLenum GLAPIENTRY qt_glClientWaitSync(GLsync sync, GLbitfield flags, GLuint64 timeout) {
    return s_glCore->glClientWaitSync(sync, flags, timeout);
}

void GLAPIENTRY qt_glDeleteSync(GLsync sync) {
    s_glCore->glDeleteSync(sync);
}

GLboolean GLAPIENTRY qt_glIsSync(GLsync sync) {
    return s_glCore->glIsSync(sync);
}

GLsync GLAPIENTRY qt_glFenceSync(GLenum condition, GLbitfield flags) {
    return s_glCore->glFenceSync(condition, flags);
}

void GLAPIENTRY qt_glProvokingVertex(GLenum mode) {
    s_glCore->glProvokingVertex(mode);
}

void GLAPIENTRY qt_glMultiDrawElementsBaseVertex(GLenum mode, const GLsizei *count, GLenum type, const GLvoid* const *indices, GLsizei drawcount, const GLint *basevertex) {
    s_glCore->glMultiDrawElementsBaseVertex(mode, count, type, indices, drawcount, basevertex);
}

void GLAPIENTRY qt_glDrawElementsInstancedBaseVertex(GLenum mode, GLsizei count, GLenum type, const GLvoid *indices, GLsizei instancecount, GLint basevertex) {
    s_glCore->glDrawElementsInstancedBaseVertex(mode, count, type, indices, instancecount, basevertex);
}

void GLAPIENTRY qt_glDrawRangeElementsBaseVertex(GLenum mode, GLuint start, GLuint end, GLsizei count, GLenum type, const GLvoid *indices, GLint basevertex) {
    s_glCore->glDrawRangeElementsBaseVertex(mode, start, end, count, type, indices, basevertex);
}

void GLAPIENTRY qt_glDrawElementsBaseVertex(GLenum mode, GLsizei count, GLenum type, const GLvoid *indices, GLint basevertex) {
    s_glCore->glDrawElementsBaseVertex(mode, count, type, indices, basevertex);
}

void GLAPIENTRY qt_glFramebufferTexture(GLenum target, GLenum attachment, GLuint texture, GLint level) {
    s_glCore->glFramebufferTexture(target, attachment, texture, level);
}

void GLAPIENTRY qt_glGetBufferParameteri64v(GLenum target, GLenum pname, GLint64 *params) {
    s_glCore->glGetBufferParameteri64v(target, pname, params);
}

void GLAPIENTRY qt_glGetInteger64i_v(GLenum target, GLuint index, GLint64 *data) {
    s_glCore->glGetInteger64i_v(target, index, data);
}

void GLAPIENTRY qt_glVertexAttribP4uiv(GLuint index, GLenum type, GLboolean normalized, const GLuint *value) {
    s_glCore->glVertexAttribP4uiv(index, type, normalized, value);
}

void GLAPIENTRY qt_glVertexAttribP4ui(GLuint index, GLenum type, GLboolean normalized, GLuint value) {
    s_glCore->glVertexAttribP4ui(index, type, normalized, value);
}

void GLAPIENTRY qt_glVertexAttribP3uiv(GLuint index, GLenum type, GLboolean normalized, const GLuint *value) {
    s_glCore->glVertexAttribP3uiv(index, type, normalized, value);
}

void GLAPIENTRY qt_glVertexAttribP3ui(GLuint index, GLenum type, GLboolean normalized, GLuint value) {
    s_glCore->glVertexAttribP3ui(index, type, normalized, value);
}

void GLAPIENTRY qt_glVertexAttribP2uiv(GLuint index, GLenum type, GLboolean normalized, const GLuint *value) {
    s_glCore->glVertexAttribP2uiv(index, type, normalized, value);
}

void GLAPIENTRY qt_glVertexAttribP2ui(GLuint index, GLenum type, GLboolean normalized, GLuint value) {
    s_glCore->glVertexAttribP2ui(index, type, normalized, value);
}

void GLAPIENTRY qt_glVertexAttribP1uiv(GLuint index, GLenum type, GLboolean normalized, const GLuint *value) {
    s_glCore->glVertexAttribP1uiv(index, type, normalized, value);
}

void GLAPIENTRY qt_glVertexAttribP1ui(GLuint index, GLenum type, GLboolean normalized, GLuint value) {
    s_glCore->glVertexAttribP1ui(index, type, normalized, value);
}

void GLAPIENTRY qt_glSecondaryColorP3uiv(GLenum type, const GLuint *color) {
    s_glCore->glSecondaryColorP3uiv(type, color);
}

void GLAPIENTRY qt_glSecondaryColorP3ui(GLenum type, GLuint color) {
    s_glCore->glSecondaryColorP3ui(type, color);
}

void GLAPIENTRY qt_glColorP4uiv(GLenum type, const GLuint *color) {
    s_glCore->glColorP4uiv(type, color);
}

void GLAPIENTRY qt_glColorP4ui(GLenum type, GLuint color) {
    s_glCore->glColorP4ui(type, color);
}

void GLAPIENTRY qt_glColorP3uiv(GLenum type, const GLuint *color) {
    s_glCore->glColorP3uiv(type, color);
}

void GLAPIENTRY qt_glColorP3ui(GLenum type, GLuint color) {
    s_glCore->glColorP3ui(type, color);
}

void GLAPIENTRY qt_glNormalP3uiv(GLenum type, const GLuint *coords) {
    s_glCore->glNormalP3uiv(type, coords);
}

void GLAPIENTRY qt_glNormalP3ui(GLenum type, GLuint coords) {
    s_glCore->glNormalP3ui(type, coords);
}

void GLAPIENTRY qt_glMultiTexCoordP4uiv(GLenum texture, GLenum type, const GLuint *coords) {
    s_glCore->glMultiTexCoordP4uiv(texture, type, coords);
}

void GLAPIENTRY qt_glMultiTexCoordP4ui(GLenum texture, GLenum type, GLuint coords) {
    s_glCore->glMultiTexCoordP4ui(texture, type, coords);
}

void GLAPIENTRY qt_glMultiTexCoordP3uiv(GLenum texture, GLenum type, const GLuint *coords) {
    s_glCore->glMultiTexCoordP3uiv(texture, type, coords);
}

void GLAPIENTRY qt_glMultiTexCoordP3ui(GLenum texture, GLenum type, GLuint coords) {
    s_glCore->glMultiTexCoordP3ui(texture, type, coords);
}

void GLAPIENTRY qt_glMultiTexCoordP2uiv(GLenum texture, GLenum type, const GLuint *coords) {
    s_glCore->glMultiTexCoordP2uiv(texture, type, coords);
}

void GLAPIENTRY qt_glMultiTexCoordP2ui(GLenum texture, GLenum type, GLuint coords) {
    s_glCore->glMultiTexCoordP2ui(texture, type, coords);
}

void GLAPIENTRY qt_glMultiTexCoordP1uiv(GLenum texture, GLenum type, const GLuint *coords) {
    s_glCore->glMultiTexCoordP1uiv(texture, type, coords);
}

void GLAPIENTRY qt_glMultiTexCoordP1ui(GLenum texture, GLenum type, GLuint coords) {
    s_glCore->glMultiTexCoordP1ui(texture, type, coords);
}

void GLAPIENTRY qt_glTexCoordP4uiv(GLenum type, const GLuint *coords) {
    s_glCore->glTexCoordP4uiv(type, coords);
}

void GLAPIENTRY qt_glTexCoordP4ui(GLenum type, GLuint coords) {
    s_glCore->glTexCoordP4ui(type, coords);
}

void GLAPIENTRY qt_glTexCoordP3uiv(GLenum type, const GLuint *coords) {
    s_glCore->glTexCoordP3uiv(type, coords);
}

void GLAPIENTRY qt_glTexCoordP3ui(GLenum type, GLuint coords) {
    s_glCore->glTexCoordP3ui(type, coords);
}

void GLAPIENTRY qt_glTexCoordP2uiv(GLenum type, const GLuint *coords) {
    s_glCore->glTexCoordP2uiv(type, coords);
}

void GLAPIENTRY qt_glTexCoordP2ui(GLenum type, GLuint coords) {
    s_glCore->glTexCoordP2ui(type, coords);
}

void GLAPIENTRY qt_glTexCoordP1uiv(GLenum type, const GLuint *coords) {
    s_glCore->glTexCoordP1uiv(type, coords);
}

void GLAPIENTRY qt_glTexCoordP1ui(GLenum type, GLuint coords) {
    s_glCore->glTexCoordP1ui(type, coords);
}

void GLAPIENTRY qt_glVertexP4uiv(GLenum type, const GLuint *value) {
    s_glCore->glVertexP4uiv(type, value);
}

void GLAPIENTRY qt_glVertexP4ui(GLenum type, GLuint value) {
    s_glCore->glVertexP4ui(type, value);
}

void GLAPIENTRY qt_glVertexP3uiv(GLenum type, const GLuint *value) {
    s_glCore->glVertexP3uiv(type, value);
}

void GLAPIENTRY qt_glVertexP3ui(GLenum type, GLuint value) {
    s_glCore->glVertexP3ui(type, value);
}

void GLAPIENTRY qt_glVertexP2uiv(GLenum type, const GLuint *value) {
    s_glCore->glVertexP2uiv(type, value);
}

void GLAPIENTRY qt_glVertexP2ui(GLenum type, GLuint value) {
    s_glCore->glVertexP2ui(type, value);
}

void GLAPIENTRY qt_glGetQueryObjectui64v(GLuint id, GLenum pname, GLuint64 *params) {
    s_glCore->glGetQueryObjectui64v(id, pname, params);
}

void GLAPIENTRY qt_glGetQueryObjecti64v(GLuint id, GLenum pname, GLint64 *params) {
    s_glCore->glGetQueryObjecti64v(id, pname, params);
}

void GLAPIENTRY qt_glQueryCounter(GLuint id, GLenum target) {
    s_glCore->glQueryCounter(id, target);
}

void GLAPIENTRY qt_glGetSamplerParameterIuiv(GLuint sampler, GLenum pname, GLuint *params) {
    s_glCore->glGetSamplerParameterIuiv(sampler, pname, params);
}

void GLAPIENTRY qt_glGetSamplerParameterfv(GLuint sampler, GLenum pname, GLfloat *params) {
    s_glCore->glGetSamplerParameterfv(sampler, pname, params);
}

void GLAPIENTRY qt_glGetSamplerParameterIiv(GLuint sampler, GLenum pname, GLint *params) {
    s_glCore->glGetSamplerParameterIiv(sampler, pname, params);
}

void GLAPIENTRY qt_glGetSamplerParameteriv(GLuint sampler, GLenum pname, GLint *params) {
    s_glCore->glGetSamplerParameteriv(sampler, pname, params);
}

void GLAPIENTRY qt_glSamplerParameterIuiv(GLuint sampler, GLenum pname, const GLuint *param) {
    s_glCore->glSamplerParameterIuiv(sampler, pname, param);
}

void GLAPIENTRY qt_glSamplerParameterIiv(GLuint sampler, GLenum pname, const GLint *param) {
    s_glCore->glSamplerParameterIiv(sampler, pname, param);
}

void GLAPIENTRY qt_glSamplerParameterfv(GLuint sampler, GLenum pname, const GLfloat *param) {
    s_glCore->glSamplerParameterfv(sampler, pname, param);
}

void GLAPIENTRY qt_glSamplerParameterf(GLuint sampler, GLenum pname, GLfloat param) {
    s_glCore->glSamplerParameterf(sampler, pname, param);
}

void GLAPIENTRY qt_glSamplerParameteriv(GLuint sampler, GLenum pname, const GLint *param) {
    s_glCore->glSamplerParameteriv(sampler, pname, param);
}

void GLAPIENTRY qt_glSamplerParameteri(GLuint sampler, GLenum pname, GLint param) {
    s_glCore->glSamplerParameteri(sampler, pname, param);
}

void GLAPIENTRY qt_glBindSampler(GLuint unit, GLuint sampler) {
    s_glCore->glBindSampler(unit, sampler);
}

GLboolean GLAPIENTRY qt_glIsSampler(GLuint sampler) {
    return s_glCore->glIsSampler(sampler);
}

void GLAPIENTRY qt_glDeleteSamplers(GLsizei count, const GLuint *samplers) {
    s_glCore->glDeleteSamplers(count, samplers);
}

void GLAPIENTRY qt_glGenSamplers(GLsizei count, GLuint *samplers) {
    s_glCore->glGenSamplers(count, samplers);
}

GLint GLAPIENTRY qt_glGetFragDataIndex(GLuint program, const GLchar *name) {
    return s_glCore->glGetFragDataIndex(program, name);
}

void GLAPIENTRY qt_glBindFragDataLocationIndexed(GLuint program, GLuint colorNumber, GLuint index, const GLchar *name) {
    s_glCore->glBindFragDataLocationIndexed(program, colorNumber, index, name);
}

void GLAPIENTRY qt_glVertexAttribDivisor(GLuint index, GLuint divisor) {
    s_glCore->glVertexAttribDivisor(index, divisor);
}
// clang-format on
