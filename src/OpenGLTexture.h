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
		GLuint textureId = ZERO_UINT;
		Vec2i size;

	public:

		API_INTERFACE OpenGLTexture()
		{
		}

		API_INTERFACE inline OpenGLTexture* init()
		{
			glGenTextures(1, &textureId);
			return this;
		}

		API_INTERFACE inline OpenGLTexture* use()
		{
			glBindTexture(GL_TEXTURE_2D, textureId);
			return this;
		}

		API_INTERFACE inline OpenGLTexture* setProperty(GLenum name, GLint value)
		{
			glTexParameteri(GL_TEXTURE_2D, name, value);
			return this;
		}

		API_INTERFACE static OpenGLTexture* createFromFramebuffer(GLenum framebuffer = GL_BACK)
		{
			Vec2f size = RendererSettings::getInstance()->getSize();
			sp_uchar* data = Framebuffer::getFramebuffer(framebuffer);

			OpenGLTexture* texture = sp_mem_new(OpenGLTexture)();
			texture->use()
				->setProperty(GL_TEXTURE_MIN_FILTER, GL_LINEAR)
				->setProperty(GL_TEXTURE_MAG_FILTER, GL_LINEAR)
				->setData(data, Vec2i((sp_int) size.x, (sp_int)size.y), GL_RGBA);
			
			sp_mem_release(data);
			return texture;
		}

		API_INTERFACE inline void setData(const sp_uchar* data, const Vec2i& size, GLint colorFormat)
		{
			this->size = size;
			glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
			glTexImage2D(GL_TEXTURE_2D, 0, colorFormat, size.x, size.y, 0, colorFormat, GL_UNSIGNED_BYTE, data);
		}

		API_INTERFACE inline void resize(const Vec2i& size)
		{
			this->size = size;
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
			if (textureId != ZERO_UINT)
			{
				glDeleteTextures(ONE_SIZE, &textureId);
				textureId = ZERO_UINT;
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
