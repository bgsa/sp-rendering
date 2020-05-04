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
		Vec2f imageSize = RendererSettings::getInstance()->getSize();
		sp_uchar* data = (sp_uchar*) sp_mem_calloc(FOUR_UINT * (sp_uint)imageSize.x * (sp_uint)imageSize.y, SIZEOF_UCHAR);

		glPixelStorei(GL_PACK_ALIGNMENT, ONE_INT);

		glReadBuffer(framebuffer);

		if (glGetError() == GL_INVALID_OPERATION)
			printf("Error setting FRAMEBUFFER");

		glReadPixels(ZERO_INT, ZERO_INT, (GLsizei) imageSize.x, (GLsizei) imageSize.y, GL_RGBA, GL_UNSIGNED_BYTE, data);

		if (glGetError() != GL_NO_ERROR)
			printf("Unknown error Reading Pixels");

		return data;
	}

	void Framebuffer::saveImage(std::string filename, sp_uchar* pixels, sp_float width, sp_float height)
	{
		//stbi_write_png(filename.c_str(), width, height, 4, pixels, 0);
	}

	void Framebuffer::saveFrambebuffer(std::string filename, GLenum framebuffer)
	{
		Vec2f imageSize = RendererSettings::getInstance()->getSize();
		sp_uchar* data = getFramebuffer(framebuffer);

		Framebuffer::saveImage(filename, data, imageSize.x, imageSize.y);

		delete[] data;
	}
}