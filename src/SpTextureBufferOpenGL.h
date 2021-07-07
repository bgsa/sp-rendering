#ifdef OPENGL_ENABLED

#ifndef SP_TEXTURE_BUFFER_OPENGL_HEADER
#define SP_TEXTURE_BUFFER_OPENGL_HEADER

#include "SpectrumRendering.h"
#include "SpGpuTextureBuffer.h"

namespace NAMESPACE_RENDERING
{
	class SpTextureBufferOpenGL
		: public SpGpuTextureBuffer
	{
	public:

		API_INTERFACE inline SpTextureBufferOpenGL()
		{
			glGenTextures(1, &_textureId);
			glGenBuffers(1, &_bufferId);
		}

		API_INTERFACE inline void updateData(const sp_size size, const void* buffer, sp_uint usage = GL_DYNAMIC_DRAW) override
		{
			sp_opengl_check(glBindBuffer(GL_TEXTURE_BUFFER, _bufferId));
			sp_opengl_check(glBufferData(GL_TEXTURE_BUFFER, size, buffer, usage));
			sp_opengl_check(glTexBuffer(GL_TEXTURE_BUFFER, GL_RGBA32F, _bufferId));
		}

		API_INTERFACE inline SpTextureBufferOpenGL* use() override
		{
			sp_opengl_check(glBindBuffer(GL_TEXTURE_BUFFER, _bufferId));

			sp_opengl_check(glActiveTexture(GL_TEXTURE0));
			sp_opengl_check(glBindTexture(GL_TEXTURE_BUFFER, _textureId));

			return this;
		}

		API_INTERFACE virtual void disable() override
		{
			sp_opengl_check(glBindBuffer(GL_TEXTURE_BUFFER, 0));
			sp_opengl_check(glBindTexture(GL_TEXTURE_BUFFER, 0));
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

		~SpTextureBufferOpenGL()
		{
			dispose();
		}

	};
}

#endif // SP_TEXTURE_BUFFER_OPENGL_HEADER

#endif // OPENGL_ENABLED
