#ifndef SP_RENDERING_API_OPENGL_HEADER
#define SP_RENDERING_API_OPENGL_HEADER

#include "SpectrumRendering.h"
#include "SpRenderingAPI.h"
#include "OpenGLShader.h"
#include "SpGpuBuffer.h"
#include "OpenGLBuffer.h"

namespace NAMESPACE_RENDERING
{
	class SpRenderingAPIOpenGL
		: public SpRenderingAPI
	{
	public:

		API_INTERFACE inline void enable(const sp_uint propertyId) override
		{
			sp_opengl_check(glEnable(propertyId));
		}

		API_INTERFACE inline void disable(const sp_uint propertyId) override
		{
			sp_opengl_check(glDisable(propertyId));
		}

		API_INTERFACE inline void enableLineSmooth() override
		{
			sp_opengl_check(glEnable(GL_LINE_SMOOTH));
		}

		API_INTERFACE inline void enableDepthTest() override
		{
			sp_opengl_check(glEnable(GL_DEPTH_TEST));
		}

		API_INTERFACE inline void enableCullingFace() override
		{
			sp_opengl_check(glEnable(GL_CULL_FACE));
		}

		API_INTERFACE inline void enablePolygonOffsetFill() override
		{
			sp_opengl_check(glEnable(GL_POLYGON_OFFSET_FILL));
		}

		API_INTERFACE inline void setPolygonOffset(const sp_float factor, const sp_float units) override
		{
			sp_opengl_check(glPolygonOffset(factor, units));
		}

		API_INTERFACE inline void clearFramebuffer() override
		{
			sp_opengl_check(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT));
		}

		API_INTERFACE inline void clearColor(const SpColorRGBA& rgba) override
		{
			sp_opengl_check(glClearColor(rgba.red, rgba.green, rgba.blue, rgba.alpha));
		}

		API_INTERFACE inline void setViewport(const SpViewportData& viewport) override
		{
			sp_opengl_check(glViewport(viewport.x, viewport.y, viewport.width, viewport.height));
		}

		API_INTERFACE inline void setScissor(const SpRect<sp_int>& area) override
		{
			sp_opengl_check(glScissor(area.x, area.y, area.width, area.height));
		}

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

#endif // SP_RENDERING_API_OPENGL_HEADER