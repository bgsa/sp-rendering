#ifdef OPENGL_ENABLED

#ifndef OPENGL_TEXTURE_BUFFER_HEADER
#define OPENGL_TEXTURE_BUFFER_HEADER

#include "SpectrumRendering.h"

namespace NAMESPACE_RENDERING
{
	class OpenGLTextureBuffer
		: Object
	{
	private:
		GLuint textureId = ZERO_UINT;
		GLuint dataBuffer = ZERO_UINT;

	public:

		API_INTERFACE OpenGLTextureBuffer()
		{
			glGenTextures(1, &textureId);
			glGenBuffers(1, &dataBuffer);
		}

		API_INTERFACE inline void setData(const sp_size size, const void* buffer, sp_uint usage = GL_DYNAMIC_DRAW)
		{
			glBindBuffer(GL_TEXTURE_BUFFER, dataBuffer);
			glBufferData(GL_TEXTURE_BUFFER, size, buffer, usage);
			glTexBuffer(GL_TEXTURE_BUFFER, GL_RGBA32F, dataBuffer);
		}

		API_INTERFACE inline OpenGLTextureBuffer* use()
		{
			glBindBuffer(GL_TEXTURE_BUFFER, dataBuffer);

			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_BUFFER, textureId);

			return this;
		}

		API_INTERFACE inline sp_uint getTextureId()
		{
			return textureId;
		}

		API_INTERFACE inline sp_uint getBufferId()
		{
			return dataBuffer;
		}

		API_INTERFACE const sp_char* toString() override
		{
			return "OpenGL Texture Buffer";
		}

		API_INTERFACE void dispose() override
		{
			if (textureId != ZERO_UINT)
			{
				glDeleteTextures(ONE_SIZE, &textureId);
				textureId = ZERO_UINT;
			}

			if (dataBuffer != ZERO_UINT)
			{
				glDeleteBuffers(ONE_SIZE, &dataBuffer);
				dataBuffer = ZERO_UINT;
			}
		}

		~OpenGLTextureBuffer()
		{
			dispose();
		}

	};
}

#endif // OPENGL_TEXTURE_BUFFER_HEADER

#endif // OPENGL_ENABLED
