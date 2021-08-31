#ifdef OPENGL_ENABLED

#ifndef SP_SHADER_STORAGE_BUFFER_OBJECT_OPENGL_HEADER
#define SP_SHADER_STORAGE_BUFFER_OBJECT_OPENGL_HEADER

#include "SpectrumRendering.h"
#include "SpGpuBuffer.h"

namespace NAMESPACE_RENDERING
{
	class SpShaderStorageBufferObjectOpenGL 
		: public SpGpuBuffer
	{	
	public:

		/// <summary>
		/// Default constructor
		/// </summary>
		/// <returns></returns>
		API_INTERFACE inline SpShaderStorageBufferObjectOpenGL()
		{
			sp_opengl_check(glGenBuffers(1, &_id));
		}

		/// <summary>
		/// Use/Bind this buffer
		/// </summary>
		/// <returns></returns>
		API_INTERFACE inline SpGpuBuffer* use() override
		{
			sp_opengl_check(glBindBuffer(GL_SHADER_STORAGE_BUFFER, _id));

			return this;
		}

		/// <summary>
		/// Update data in gpu buffer storage
		/// </summary>
		/// <param name="size"></param>
		/// <param name="data"></param>
		/// <param name="usageType"></param>
		/// <returns></returns>
		API_INTERFACE inline  SpGpuBuffer* updateData(const sp_size size, const void* data, const sp_int usageType) override
		{
			sp_assert(size != 0, "InvalidArgumentException");
			sp_opengl_check(glBufferData(GL_SHADER_STORAGE_BUFFER, size, data, usageType));

			return this;
		}

		/// <summary>
		/// Disable this buffer
		/// </summary>
		/// <returns></returns>
		API_INTERFACE inline void disable() override
		{
			sp_opengl_check(glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0));
		}

		/// <summary>
		/// Release all allocated resources
		/// </summary>
		/// <returns></returns>
		API_INTERFACE inline void dispose() override
		{
			sp_opengl_check(glDeleteBuffers(ONE_INT, &_id));
		}

	};
}

#endif // SP_SHADER_STORAGE_BUFFER_OBJECT_OPENGL_HEADER

#endif // !OPENGL_ENABLED