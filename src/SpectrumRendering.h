#ifndef SPECTRUM_RENDERING_HEADER
#define SPECTRUM_RENDERING_HEADER

#ifndef NAMESPACE_RENDERING
	#define NAMESPACE_RENDERING SpRendering
#endif // NAMESPACE_RENDERING

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

#include "RendererSettings.h"

#endif // !SPECTRUM_RENDERING_HEADER
