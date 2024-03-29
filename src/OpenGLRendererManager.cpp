#ifdef OPENGL_ENABLED

#include "OpenGLRendererManager.h"
#include "SpLogger.h"

namespace NAMESPACE_RENDERING
{

	void OpenGLRendererManager::resize(sp_float width, sp_float height)
	{
		if (width == ZERO_FLOAT || height == ZERO_FLOAT)
			return;

		if (_viewport.width == width && _viewport.height == height)
			return;

		_viewport.resize((sp_int)width, (sp_int)height);

		sp_opengl_check(glViewport(_viewport.x, _viewport.y, _viewport.width, _viewport.height));
		sp_opengl_check(glScissor(_viewport.x, _viewport.y, _viewport.width, _viewport.height));

		_camera->aspectRatio(_viewport.aspectRatio());
	}

	void OpenGLRendererManager::init(SpCamera* camera)
	{
		this->_camera = camera;

		sp_log_error1s("OpenGL Vendor: ");
		sp_log_error1s(GLConfig::getGLVendor().c_str()); sp_log_newline();

		sp_log_error1s("OpenGL Version: ");
		sp_log_error1s(GLConfig::getGLVersion().c_str()); sp_log_newline();

		sp_log_error1s("OpenGLSL Version: ");
		sp_log_error1s(GLConfig::getGLShadingLanguageVersion().c_str()); sp_log_newline();

		sp_log_error1s("OpenGL Renderer: ");
		sp_log_error1s(GLConfig::getGLRenderer().c_str()); sp_log_newline();

		sp_log_error1s("OpenGL Extensions: ");
		for (std::string extension : GLConfig::getGLExtensions())
		{
			sp_log_error1s(extension.c_str());
			sp_log_newline();
		}

		sp_opengl_check(glEnable(GL_SCISSOR_TEST));
		sp_opengl_check(glEnable(GL_DEPTH_TEST)); //elimina os v�rtices que sobrepoem outros v�rtices quando est�o no mesmo eixo Z.
		sp_opengl_check(glEnable(GL_BLEND));								  //enable alpha color
		sp_opengl_check(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));    //enable alpha color
		sp_opengl_check(glEnable(GL_LINE_SMOOTH));

		sp_opengl_check(glViewport(_viewport.x, _viewport.y, _viewport.width, _viewport.height));
		sp_opengl_check(glScissor(_viewport.x, _viewport.y, _viewport.width, _viewport.height));

		SpIRendererManager::init(camera);
	}

	void OpenGLRendererManager::preRender()
	{
	}

	void OpenGLRendererManager::render()
	{
		_camera->update(_viewport.aspectRatio());

		SpRenderData renderData;
		renderData.projectionMatrix = _camera->projectionMatrix();
		renderData.viewMatrix = _camera->viewMatrix();
		renderData.viewport = &_viewport;

		sp_opengl_check(glViewport(_viewport.x, _viewport.y, _viewport.width, _viewport.height));
		sp_opengl_check(glScissor(_viewport.x, _viewport.y, _viewport.width, _viewport.height));
		sp_opengl_check(glClearColor(0.5f, 0.5f, 0.5f, 1.0f));
		sp_opengl_check(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT));

		sp_opengl_check(glEnable(GL_LINE_SMOOTH));
		sp_opengl_check(glEnable(GL_DEPTH_TEST));
		sp_opengl_check(glEnable(GL_CULL_FACE));
		sp_opengl_check(glEnable(GL_POLYGON_OFFSET_FILL));
		sp_opengl_check(glPolygonOffset(1.0f, 1.0f));
		//sp_opengl_check(glPolygonMode(GL_FRONT_AND_BACK, GL_LINE));

		render3D(renderData);

		_camera->hudProjectionMatrix((sp_float)_viewport.width, (sp_float)_viewport.height, renderData.projectionMatrix);

		sp_opengl_check(glDisable(GL_DEPTH_TEST));
		sp_opengl_check(glDisable(GL_CULL_FACE));

		render2D(renderData);
	}

	void OpenGLRendererManager::postRender()
	{
	}

	void OpenGLRendererManager::dispose()
	{
		if (_camera != NULL)
		{
			sp_mem_delete(_camera, SpCamera);
			_camera = nullptr;
		}
	}
}

#endif // OPENGL_ENABLED