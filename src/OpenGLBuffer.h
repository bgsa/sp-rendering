#ifndef OPENGL_BUFFER_HEADER
#define OPENGL_BUFFER_HEADER

#include "SpectrumRendering.h"

namespace NAMESPACE_RENDERING
{
	class OpenGLBuffer
		: public Object
	{
	private:
		GLuint bufferId;
		GLenum type = GL_ARRAY_BUFFER;

	public:

		/// <summary>
		/// Create a new OpenGL Buffer
		/// </summary>
		API_INTERFACE inline OpenGLBuffer(GLenum bufferType = GL_ARRAY_BUFFER)
		{
			type = bufferType;
			glGenBuffers(ONE_INT, &bufferId);
		}

		/// <summary>
		/// Create a new OpenGL Buffer with data
		/// </summary>
		API_INTERFACE inline OpenGLBuffer(sp_size size, const void* data, GLenum bufferType = GL_ARRAY_BUFFER, GLenum usageType = GL_STATIC_DRAW)
		{
			type = bufferType;
			glGenBuffers(ONE_INT, &bufferId);
			glBindBuffer(type, bufferId);
			glBufferData(type, size, data, usageType);
		}

		/// <summary>
		/// Set the type og buffer: GL_ARRAY_BUFFER, GL_ELEMENT_ARRAY_BUFFER, GL_TEXTURE_COORD_ARRAY_BUFFER_BINDING
		/// </summary>
		API_INTERFACE inline OpenGLBuffer* setType(GLenum bufferType = GL_ARRAY_BUFFER)
		{
			type = bufferType;
			return this;
		}

		API_INTERFACE inline OpenGLBuffer* use()
		{
			glBindBuffer(type, bufferId);
			return this;
		}

		API_INTERFACE inline OpenGLBuffer* setData(sp_size size, void* data, GLenum usageType = GL_STATIC_DRAW)
		{
			glBufferData(type, size, data, usageType);
			return this;
		}

		API_INTERFACE inline const sp_char* toString() override
		{
			return "OpenGLBuffer";
		}

		API_INTERFACE inline void dispose() override
		{
			glDeleteBuffers(ONE_INT, &bufferId);
		}

		API_INTERFACE ~OpenGLBuffer()
		{
			glDeleteBuffers(ONE_INT, &bufferId);
		}

	};
}

#endif // OPENGL_BUFFER_HEADER