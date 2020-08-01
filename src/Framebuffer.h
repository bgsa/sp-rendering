#ifndef FRAMBEBUFFER_HEADER
#define FRAMBEBUFFER_HEADER

#include "SpectrumRendering.h"

namespace NAMESPACE_RENDERING
{
	class Framebuffer
	{
	public:
				
		API_INTERFACE static void getFramebuffer(sp_uchar* data, GLenum framebuffer = GL_BACK)
		{
			GLint viewport[4];
			glGetIntegerv(GL_VIEWPORT, viewport);

			glPixelStorei(GL_PACK_ALIGNMENT, ONE_INT);

			glReadBuffer(framebuffer);

			sp_assert(glGetError() == GL_NO_ERROR, "Error setting FRAMEBUFFER");

			glReadPixels(ZERO_INT, ZERO_INT, viewport[2], viewport[3], GL_RGBA, GL_UNSIGNED_BYTE, data);

			sp_assert(glGetError() == GL_NO_ERROR, "Unknown error Reading Pixels");
		}

		API_INTERFACE static void saveFrambebuffer(std::string filename, GLenum framebuffer)
		{
			GLint viewport[4];
			glGetIntegerv(GL_VIEWPORT, viewport);

			sp_uchar* data = ALLOC_ARRAY(sp_uchar, FOUR_UINT * viewport[2] * viewport[3]);
			getFramebuffer(data, framebuffer);

			Framebuffer::saveImage(filename, data, viewport[2], viewport[3]);

			ALLOC_RELEASE(data);
		}

		API_INTERFACE static void saveImage(std::string filename, sp_uchar* pixels, sp_int width, sp_int height)
		{
			//stbi_write_png(filename.c_str(), width, height, 4, pixels, 0);
		}


	};
}

#endif // FRAMBEBUFFER_HEADER