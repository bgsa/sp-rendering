#ifndef OPENGL_BUFFER_HEADER
#define OPENGL_BUFFER_HEADER

#include "SpectrumRendering.h"
#include "SpGpuBuffer.h"

namespace NAMESPACE_RENDERING
{
	class OpenGLBuffer
		: public SpGpuBuffer
	{
	public:

		/// <summary>
		/// Create a new OpenGL Buffer
		/// </summary>
		API_INTERFACE inline OpenGLBuffer(GLenum bufferType = GL_ARRAY_BUFFER)
		{
			_type = bufferType;
			sp_opengl_check(glGenBuffers(ONE_INT, &_id));
		}

		/// <summary>
		/// Create a new OpenGL Buffer with data
		/// </summary>
		API_INTERFACE inline OpenGLBuffer(sp_size size, const void* data, GLenum bufferType = GL_ARRAY_BUFFER, GLenum usageType = GL_STATIC_DRAW)
		{
			sp_assert(size > 0, "InvalidArgumentException");
			sp_assert(data != nullptr, "InvalidArgumentException");

			_type = bufferType;
			sp_opengl_check(glGenBuffers(ONE_INT, &_id));
			sp_opengl_check(glBindBuffer(_type, _id));
			sp_opengl_check(glBufferData(_type, size, data, usageType));
		}

		API_INTERFACE inline OpenGLBuffer* use() override
		{
			sp_opengl_check(glBindBuffer(_type, _id));
			return this;
		}

		API_INTERFACE inline void disable() override
		{
			sp_opengl_check(glBindBuffer(_type, 0));
		}

		API_INTERFACE inline OpenGLBuffer* updateData(const sp_size size, const void* data, const sp_int usageType = GL_STATIC_DRAW) override
		{
			sp_assert(size > 0, "InvalidArgumentException");
			sp_assert(data != nullptr, "InvalidArgumentException");

			sp_opengl_check(glBufferData(_type, size, data, usageType));
			return this;
		}

		API_INTERFACE inline void dispose()
		{
			sp_opengl_check(glDeleteBuffers(ONE_INT, &_id));
		}

		API_INTERFACE ~OpenGLBuffer()
		{
			dispose();
		}

	};
}

#endif // OPENGL_BUFFER_HEADER