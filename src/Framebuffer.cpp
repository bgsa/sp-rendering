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
		sp_uchar* data = new sp_uchar[FOUR_INT * imageSize.x * imageSize.y];

		glPixelStorei(GL_PACK_ALIGNMENT, ONE_INT);

		glReadBuffer(framebuffer);

		if (glGetError() == GL_INVALID_OPERATION)
			printf("Error setting FRAMEBUFFER");

		glReadPixels(ZERO_INT, ZERO_INT, imageSize.x, imageSize.y, GL_RGBA, GL_UNSIGNED_BYTE, data);

		if (glGetError() != GL_NO_ERROR)
			printf("Unknown error Reading Pixels");

		return data;
	}

	sp_uint Framebuffer::getFramebufferTexture(GLenum framebuffer)
	{
		Vec2f imageSize = RendererSettings::getInstance()->getSize();
		sp_uchar* imageBuffer = Framebuffer::getFramebuffer(framebuffer);

		GLuint imageTexture;
		glGenTextures(1, &imageTexture);
		glBindTexture(GL_TEXTURE_2D, imageTexture);

		// Setup filtering parameters for display
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		// Upload pixels into texture
		glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageSize.x, imageSize.y, 0, GL_RGBA, GL_UNSIGNED_BYTE, imageBuffer);

		delete[] imageBuffer;
		return imageTexture;
	}

	void Framebuffer::saveImage(std::string filename, sp_uchar* pixels, sp_int width, sp_int height)
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