#include "Framebuffer.h"

/*
#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <stb/stb_image_write.h>
*/

namespace NAMESPACE_RENDERING
{
	sp_uchar* Framebuffer::getFramebuffer(GLenum framebuffer)
	{
		GLint viewport[4];
		glGetIntegerv(GL_VIEWPORT, viewport);

		sp_uchar* data = (sp_uchar*) sp_mem_calloc(FOUR_UINT * viewport[2] * viewport[3], SIZEOF_UCHAR);

		glPixelStorei(GL_PACK_ALIGNMENT, ONE_INT);

		glReadBuffer(framebuffer);

		if (glGetError() == GL_INVALID_OPERATION)
			printf("Error setting FRAMEBUFFER");

		glReadPixels(ZERO_INT, ZERO_INT, viewport[2], viewport[3], GL_RGBA, GL_UNSIGNED_BYTE, data);

		if (glGetError() != GL_NO_ERROR)
			printf("Unknown error Reading Pixels");

		return data;
	}

	void Framebuffer::saveImage(std::string filename, sp_uchar* pixels, sp_int width, sp_int height)
	{
		//stbi_write_png(filename.c_str(), width, height, 4, pixels, 0);
	}

	void Framebuffer::saveFrambebuffer(std::string filename, GLenum framebuffer)
	{
		GLint viewport[4];
		glGetIntegerv(GL_VIEWPORT, viewport);

		sp_uchar* data = getFramebuffer(framebuffer);

		Framebuffer::saveImage(filename, data, viewport[2], viewport[3]);

		delete[] data;
	}
}