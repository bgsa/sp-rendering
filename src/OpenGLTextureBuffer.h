#ifdef OPENGL_ENABLED

#ifndef OPENGL_TEXTURE_BUFFER_HEADER
#define OPENGL_TEXTURE_BUFFER_HEADER

#include "SpectrumRendering.h"
#include "SpGpuTextureBuffer.h"

namespace NAMESPACE_RENDERING
{
	class OpenGLTextureBuffer
		: public SpGpuTextureBuffer
	{
	public:

		API_INTERFACE OpenGLTextureBuffer()
		{
			glGenTextures(1, &_textureId);
			glGenBuffers(1, &_bufferId);
		}

		API_INTERFACE inline void updateData(const sp_size size, const void* buffer, sp_uint usage = GL_DYNAMIC_DRAW) override
		{
			glBindBuffer(GL_TEXTURE_BUFFER, _bufferId);
			glBufferData(GL_TEXTURE_BUFFER, size, buffer, usage);
			glTexBuffer(GL_TEXTURE_BUFFER, GL_RGBA32F, _bufferId);
		}

		API_INTERFACE inline OpenGLTextureBuffer* use() override
		{
			glBindBuffer(GL_TEXTURE_BUFFER, _bufferId);

			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_BUFFER, _textureId);

			return this;
		}

		API_INTERFACE virtual void disable() override
		{
			glBindBuffer(GL_TEXTURE_BUFFER, 0);
			glBindTexture(GL_TEXTURE_BUFFER, 0);
		}

		API_INTERFACE const sp_char* toString() override
		{
			return "OpenGL Texture Buffer";
		}

		API_INTERFACE void dispose() override
		{
			if (_textureId != ZERO_UINT)
			{
				glDeleteTextures(ONE_SIZE, &_textureId);
				_textureId = ZERO_UINT;
			}

			if (_bufferId != ZERO_UINT)
			{
				glDeleteBuffers(ONE_SIZE, &_bufferId);
				_bufferId = ZERO_UINT;
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
