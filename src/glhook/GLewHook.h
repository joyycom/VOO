#pragma once

#ifndef __GLEW_HOOK_IMPL__
#include <QtGui/QOpenGLFunctions_3_3_Core>
void GLEW_setupOpenGLFunctions(QOpenGLFunctions_3_3_Core *functions);
#endif

void GLEW_Hook();
