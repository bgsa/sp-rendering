#ifndef SP_GPU_RENDERING_FACTORY_OPENGL_HEADER
#define SP_GPU_RENDERING_FACTORY_OPENGL_HEADER

#include "SpectrumRendering.h"
#include "SpGpuRenderingFactory.h"
#include "SpGpuBuffer.h"
#include "OpenGLBuffer.h"
#include "OpenGLTextureBuffer.h"
#include "SpGpuTextureBuffer.h"

namespace NAMESPACE_PHYSICS
{
	SpGpuRenderingFactory* instanceGpuRendering = nullptr;
}

namespace NAMESPACE_RENDERING
{
	class SpGpuRenderingFactoryOpenGL
		: public SpGpuRenderingFactory
	{
	public:
	
		API_INTERFACE static void init()
		{
			instanceGpuRendering = sp_mem_new(SpGpuRenderingFactoryOpenGL);
		}

		API_INTERFACE inline SpGpuBuffer* createBuffer(sp_int type) override
		{
			return sp_mem_new(OpenGLBuffer)(type);
		}

		API_INTERFACE inline SpGpuBuffer* createBuffer(const sp_size size, const void* data, sp_int bufferType, sp_int usageType) override
		{
			return sp_mem_new(OpenGLBuffer)(size, data, bufferType, usageType);
		}

		API_INTERFACE virtual SpGpuTextureBuffer* createTextureBuffer() override
		{
			return sp_mem_new(OpenGLTextureBuffer)();
		}

		API_INTERFACE inline const sp_char* toString() override
		{
			return "SpGpuRederingFactoryOpenGL";
		}

		API_INTERFACE inline void dispose() override { }

		API_INTERFACE ~SpGpuRenderingFactoryOpenGL()
		{
			dispose();
		}

	};
}

#endif // SP_GPU_RENDERING_FACTORY_OPENGL_HEADER