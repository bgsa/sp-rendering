#ifdef OPENGL_ENABLED

#ifndef OPENGL_TEXTURE_HEADER
#define OPENGL_TEXTURE_HEADER

#include "SpectrumRendering.h"
#include "Framebuffer.h"

namespace NAMESPACE_RENDERING
{
	class OpenGLTexture
		: Object
	{
	private:
		GLuint textureId = NULL;
		Vec2f size;

		OpenGLTexture()
		{
		}

	public:

		API_INTERFACE static OpenGLTexture* create(const sp_uchar* data, Vec2f size)
		{
			OpenGLTexture* instance = new OpenGLTexture();
			instance->size = size;

			glGenTextures(1, &instance->textureId);
			glBindTexture(GL_TEXTURE_2D, instance->textureId);

			// Setup filtering parameters for display
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

			// Upload pixels into texture
			glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, (GLsizei)size.x, (GLsizei)size.y, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);

			return instance;
		}

		API_INTERFACE static OpenGLTexture* createFromFramebuffer(GLenum framebuffer = GL_BACK)
		{
			Vec2f size = RendererSettings::getInstance()->getSize();
			sp_uchar* data = Framebuffer::getFramebuffer(framebuffer);
			OpenGLTexture* texture = create(data, size);

			delete[] data;
			return texture;
		}

		API_INTERFACE inline void updateData(const sp_uchar* data)
		{
			glBindTexture(GL_TEXTURE_2D, textureId);

			glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, (GLsizei)size.x, (GLsizei)size.y, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		}

		API_INTERFACE inline sp_uint getId()
		{
			return textureId;
		}

		API_INTERFACE const sp_char* toString() override
		{
			return "OpenGL Texture";
		}

		API_INTERFACE void dispose() override
		{
			if (textureId != NULL)
			{
				glDeleteTextures(ONE_SIZE, &textureId);
				textureId = NULL;
			}
		}

		~OpenGLTexture()
		{
			dispose();
		}

	};
}

#endif // OPENGL_TEXTURE_HEADER

#endif // OPENGL_ENABLED