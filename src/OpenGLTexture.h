#ifdef OPENGL_ENABLED

#ifndef OPENGL_TEXTURE_HEADER
#define OPENGL_TEXTURE_HEADER

#include "SpectrumRendering.h"
#include "Framebuffer.h"
#include "SpSize.h"

namespace NAMESPACE_RENDERING
{
	class OpenGLTexture
		: Object
	{
	private:
		GLuint textureId = ZERO_UINT;
		SpSize<sp_int> size;

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
			GLint viewport[4];
			glGetIntegerv(GL_VIEWPORT, viewport);

			sp_uchar* data = Framebuffer::getFramebuffer(framebuffer);

			OpenGLTexture* texture = sp_mem_new(OpenGLTexture)();
			texture->use()
				->setProperty(GL_TEXTURE_MIN_FILTER, GL_LINEAR)
				->setProperty(GL_TEXTURE_MAG_FILTER, GL_LINEAR)
				->setData(data, SpSize<sp_int>(viewport[2], viewport[3]), GL_RGBA);
			
			sp_mem_release(data);
			return texture;
		}

		API_INTERFACE inline void setData(const sp_uchar* data, const SpSize<sp_int>& size, GLint colorFormat)
		{
			this->size = size;
			glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
			glTexImage2D(GL_TEXTURE_2D, 0, colorFormat, size.width, size.height, 0, colorFormat, GL_UNSIGNED_BYTE, data);
		}

		API_INTERFACE inline void resize(const SpSize<sp_int>& size)
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
