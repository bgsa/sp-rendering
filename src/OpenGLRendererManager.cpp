#ifdef OPENGL_ENABLED

#include "OpenGLRendererManager.h"

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

		Log::info("OpenGL Vendor: "); 
		Log::info(GLConfig::getGLVendor().c_str());

		Log::info("OpenGL Version: ");
		Log::info(GLConfig::getGLVersion().c_str());

		Log::info("OpenGLSL Version: "); 
		Log::info(GLConfig::getGLShadingLanguageVersion().c_str());

		Log::info("OpenGL Renderer: ");
		Log::info(GLConfig::getGLRenderer().c_str());

		Log::info("OpenGL Extensions:");
		for (std::string extension : GLConfig::getGLExtensions())
			Log::info(extension.c_str());

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