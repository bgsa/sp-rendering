#ifndef SP_RENDERING_API_OPENGL_HEADER
#define SP_RENDERING_API_OPENGL_HEADER

#include "SpectrumRendering.h"
#include "SpRenderingAPI.h"
#include "SpShaderOpenGL.h"
#include "SpGpuBuffer.h"
#include "OpenGLBuffer.h"
#include "SpTextureBufferOpenGL.h"
#include "SpShaderStorageBufferObjectOpenGL.h"

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

		API_INTERFACE inline void disableDepthTest() override
		{
			sp_opengl_check(glDisable(GL_DEPTH_TEST));
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

		API_INTERFACE inline sp_uint bufferUsageTypeStaticDraw() override
		{
			return GL_STATIC_DRAW;
		}

		API_INTERFACE inline sp_uint bufferUsageTypeDynamicDraw() override
		{
			return GL_DYNAMIC_DRAW;
		}

		API_INTERFACE inline sp_uint typeFloatId() override
		{
			return GL_FLOAT;
		}

		API_INTERFACE inline sp_uint typeUIntId() override
		{
			return GL_UNSIGNED_INT;
		}

		API_INTERFACE inline sp_uint typeTriangleId() override
		{
			return GL_TRIANGLES;
		}

		API_INTERFACE inline sp_uint typeLinesId() override
		{
			return GL_LINES;
		}

		API_INTERFACE inline SpShader* createShader() override
		{
			return sp_mem_new(SpShaderOpenGL)();
		}

		API_INTERFACE inline SpGpuBuffer* createShaderStorageBufferObject() override
		{
			return sp_mem_new(SpShaderStorageBufferObjectOpenGL)();
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

		API_INTERFACE virtual SpGpuTextureBuffer* createTextureBuffer() override
		{
			return sp_mem_new(SpTextureBufferOpenGL)();
		}

		API_INTERFACE inline SpShader* createPrimitiveShader() override
		{
			SpShaderOpenGL* shader = sp_mem_new(SpShaderOpenGL)();
			shader->name("Primitive");

			sp_char dir[512];
			currentDirectory(dir, 512);
			sp_size dirLength = std::strlen(dir);

			directoryAddPath(dir, dirLength, "resources/shaders/opengl/primitive", 36);
			dirLength += 35;

			sp_char vsShader[512];
			directoryAddPath(dir, dirLength, "shader.vs", 9, vsShader);

			sp_char fsShader[512];
			directoryAddPath(dir, dirLength, "shader.fs", 9, fsShader);

			shader
				->buildFromFile(GL_VERTEX_SHADER, vsShader)
				->buildFromFile(GL_FRAGMENT_SHADER, fsShader)
				->link();

			return shader;
		}

		API_INTERFACE inline SpShader* createLinesShader() override
		{
			SpShaderOpenGL* shader = sp_mem_new(SpShaderOpenGL)();
			shader->name("LinesList");

			shader
				->buildFromFile(GL_VERTEX_SHADER, "resources/shaders/opengl/line-list/shader.vs")
				->buildFromFile(GL_FRAGMENT_SHADER, "resources/shaders/opengl/line-list/shader.fs")
				->link();

			return shader;
		}

		API_INTERFACE inline SpShader* createManipulatorShader() override
		{
			SpShaderOpenGL* shader = sp_mem_new(SpShaderOpenGL)();
			shader->name("Manipulator");

			shader
				->buildFromFile(GL_VERTEX_SHADER, "resources/shaders/opengl/manipulator/shader.vs")
				->buildFromFile(GL_FRAGMENT_SHADER, "resources/shaders/opengl/manipulator/shader.fs")
				->link();

			return shader;
		}

	};
}

#endif // SP_RENDERING_API_OPENGL_HEADER