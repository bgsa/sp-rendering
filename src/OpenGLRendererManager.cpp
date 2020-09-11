#ifdef OPENGL_ENABLED

#include "OpenGLRendererManager.h"
#include "SpLogger.h"

namespace NAMESPACE_RENDERING
{

	void OpenGLRendererManager::resize(sp_float width, sp_float height)
	{
		if (width == ZERO_FLOAT || height == ZERO_FLOAT)
			return;

		if (_viewport->width == width && _viewport->height == height)
			return;

		_viewport->resize((sp_int)width, (sp_int)height);

		glViewport(_viewport->x, _viewport->y, _viewport->width, _viewport->height);
		glScissor(_viewport->x, _viewport->y, _viewport->width, _viewport->height);

		_camera->updateProjectionPerspectiveAspect(_viewport->aspectRatio());
	}

	void OpenGLRendererManager::init(Camera* camera)
	{
		this->_camera = camera;

		sp_log_error1s("OpenGL Vendor: "); sp_log_newline();
		sp_log_error1s(GLConfig::getGLVendor().c_str()); sp_log_newline();

		sp_log_error1s("OpenGL Version: "); sp_log_newline();
		sp_log_error1s(GLConfig::getGLVersion().c_str()); sp_log_newline();

		sp_log_error1s("OpenGLSL Version: "); sp_log_newline();
		sp_log_error1s(GLConfig::getGLShadingLanguageVersion().c_str()); sp_log_newline();

		sp_log_error1s("OpenGL Renderer: "); sp_log_newline();
		sp_log_error1s(GLConfig::getGLRenderer().c_str()); sp_log_newline();

		sp_log_error1s("OpenGL Extensions: "); sp_log_newline();
		for (std::string extension : GLConfig::getGLExtensions())
		{
			sp_log_error1s(extension.c_str());
			sp_log_newline();
		}

		glEnable(GL_SCISSOR_TEST);
		glEnable(GL_DEPTH_TEST); //elimina os vértices que sobrepoem outros vértices quando estão no mesmo eixo Z.
		glEnable(GL_BLEND);									  //enable alpha color
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);    //enable alpha color
		glEnable(GL_LINE_SMOOTH);

		glViewport(_viewport->x, _viewport->y, _viewport->width, _viewport->height);
		glScissor(_viewport->x, _viewport->y, _viewport->width, _viewport->height);

		IRendererManager::init(camera);
	}

	void OpenGLRendererManager::preRender()
	{
	}

	void OpenGLRendererManager::render()
	{
		_camera->updateProjectionPerspectiveAspect(_viewport->aspectRatio());

		RenderData renderData;
		renderData.projectionMatrix = _camera->getProjectionMatrix();
		renderData.viewMatrix = _camera->getViewMatrix();
		renderData.viewport = _viewport;

		glViewport(_viewport->x, _viewport->y, _viewport->width, _viewport->height);
		glScissor(_viewport->x, _viewport->y, _viewport->width, _viewport->height);
		glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

		glEnable(GL_LINE_SMOOTH);
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_CULL_FACE);
		glEnable(GL_POLYGON_OFFSET_FILL);
		glPolygonOffset(1.0f, 1.0f);

		render3D(renderData);

		renderData.projectionMatrix = _camera->getHUDProjectionMatrix((sp_float)_viewport->width, (sp_float)_viewport->height);

		glDisable(GL_DEPTH_TEST);
		glDisable(GL_CULL_FACE);

		render2D(renderData);
	}

	void OpenGLRendererManager::postRender()
	{
	}

	OpenGLRendererManager::~OpenGLRendererManager()
	{
		if (_camera != NULL)
		{
			sp_mem_delete(_camera, Camera);
			_camera = nullptr;
		}
	}
}

#endif // OPENGL_ENABLED