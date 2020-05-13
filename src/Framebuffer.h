#ifndef FRAMBEBUFFER_HEADER
#define FRAMBEBUFFER_HEADER

#include "SpectrumRendering.h"

namespace NAMESPACE_RENDERING
{
	class Framebuffer
	{
	public:

		static sp_uchar* getFramebuffer(GLenum framebuffer = GL_BACK);

		static void saveFrambebuffer(std::string filename, GLenum framebuffer);

		static void saveImage(std::string filename, sp_uchar* pixels, sp_int width, sp_int height);

	};
}

#endif // FRAMBEBUFFER_HEADER