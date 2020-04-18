#ifndef SPECTRUM_RENDERING_HEADER
#define SPECTRUM_RENDERING_HEADER

#ifndef NAMESPACE_RENDERING
	#define NAMESPACE_RENDERING SpRendering
#endif

#ifdef WINDOWS
	#define ESUTIL_API  __cdecl
	#define ESCALLBACK  __cdecl
#else
	#define ESUTIL_API
	#define ESCALLBACK
#endif

#include <SpectrumPhysics.h>
#include <vector>
#include <cstring>
#include <Log.hpp>

#ifdef OPENGL_ENABLED
	#define GLEW_STATIC
	#include <GL/glew.h>
	#include <GLFW/glfw3.h>
#endif
#ifdef OPENGLES_ENABLED
	#include <GLES3/gl3.h>
#endif

using namespace NAMESPACE_PHYSICS;

typedef unsigned char  BYTE;  // 1byte
typedef unsigned short WORD;  // 2bytes
typedef unsigned long  DWORD; // 4bytes


#endif // !SPECTRUM_RENDERING_HEADER
