#ifndef FRAMBEBUFFER_HEADER
#define FRAMBEBUFFER_HEADER

#include "SpectrumRendering.h"
#include "RendererSettings.h"

namespace NAMESPACE_RENDERING
{
	class Framebuffer
	{
	public:

		static sp_uchar* getFramebuffer(GLenum framebuffer = GL_BACK);
		static sp_uint getFramebufferTexture(GLenum framebuffer = GL_BACK);

		static void saveFrambebuffer(std::string filename, GLenum framebuffer);

		static void saveImage(std::string filename, sp_uchar* pixels, sp_float width, sp_float height);

	};
}

#endif // FRAMBEBUFFER_HEADER