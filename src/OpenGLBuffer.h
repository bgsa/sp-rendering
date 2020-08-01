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
			glGenBuffers(ONE_INT, &_id);
		}

		/// <summary>
		/// Create a new OpenGL Buffer with data
		/// </summary>
		API_INTERFACE inline OpenGLBuffer(sp_size size, const void* data, GLenum bufferType = GL_ARRAY_BUFFER, GLenum usageType = GL_STATIC_DRAW)
		{
			_type = bufferType;
			glGenBuffers(ONE_INT, &_id);
			glBindBuffer(_type, _id);
			glBufferData(_type, size, data, usageType);
		}

		API_INTERFACE inline OpenGLBuffer* use() override
		{
			glBindBuffer(_type, _id);
			return this;
		}

		API_INTERFACE inline void disable() override
		{
			glBindBuffer(_type, NULL);
		}

		API_INTERFACE inline OpenGLBuffer* updateData(sp_size size, void* data, sp_int usageType = GL_STATIC_DRAW) override
		{
			glBufferData(_type, size, data, usageType);
			return this;
		}

		API_INTERFACE inline const sp_char* toString() override
		{
			return "OpenGLBuffer";
		}

		API_INTERFACE inline void dispose() override
		{
			glDeleteBuffers(ONE_INT, &_id);
		}

		API_INTERFACE ~OpenGLBuffer()
		{
			dispose();
		}

	};
}

#endif // OPENGL_BUFFER_HEADER