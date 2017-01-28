#pragma once

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <gl/gl.h>
#include <gl/gl.h>
#include "ThirdParty/glext.h"
#include "ThirdParty/wglext.h"

extern PFNGLGENBUFFERSPROC		glGenBuffers;
extern PFNGLBINDBUFFERPROC		glBindBuffer;
extern PFNGLBUFFERDATAPROC		glBufferData;
extern PFNGLGENERATEMIPMAPPROC	glGenerateMipmap;
extern PFNGLDELETEBUFFERSPROC   glDeleteBuffers;
