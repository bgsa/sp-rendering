#ifndef SP_RENDERING_FACTORY_OPENGL_HEADER
#define SP_RENDERING_FACTORY_OPENGL_HEADER

#include "SpectrumRendering.h"
#include "SpRenderingFactory.h"
#include "OpenGLShader.h"
#include "SpGpuBuffer.h"
#include "OpenGLBuffer.h"

namespace NAMESPACE_RENDERING
{
	class SpRenderingFactoryOpenGL
		: public SpRenderingFactory
	{
	public:

		API_INTERFACE inline sp_int bufferUsageTypeStaticDraw() override
		{
			return GL_STATIC_DRAW;
		}

		API_INTERFACE inline sp_int typeFloatId() override
		{
			return GL_FLOAT;
		}

		API_INTERFACE inline sp_int typeUIntId() override
		{
			return GL_UNSIGNED_INT;
		}

		API_INTERFACE inline sp_int typeTriangleId() override
		{
			return GL_TRIANGLES;
		}

		API_INTERFACE inline SpShader* createShader() override
		{
			return sp_mem_new(OpenGLShader)();
		}

		API_INTERFACE inline SpGpuBuffer* createArrayBuffer() override
		{
			return createBuffer(GL_ARRAY_BUFFER);
		}

		API_INTERFACE inline SpGpuBuffer* createElementArrayBuffer() override
		{
			return createBuffer(GL_ELEMENT_ARRAY_BUFFER);
		}

		API_INTERFACE inline SpGpuBuffer* createBuffer(const sp_int type) override
		{
			return sp_mem_new(OpenGLBuffer)(type);
		}

		API_INTERFACE inline SpShader* createPrimitiveShader() override
		{
			OpenGLShader* shader = sp_mem_new(OpenGLShader)();
			shader->name("Primitive");

			shader
				->buildFromFile(GL_VERTEX_SHADER, "resources/shaders/opengl/primitive/shader.vs")
				->buildFromFile(GL_FRAGMENT_SHADER, "resources/shaders/opengl/primitive/shader.fs")
				->link();

			return shader;
		}

	};
}

#endif // SP_RENDERING_FACTORY_OPENGL_HEADER