#ifndef SP_SCENE_RENDERER_HEADER
#define SP_SCENE_RENDERER_HEADER

#include "SpectrumRendering.h"
#include "SpSceneRendererData.h"

namespace NAMESPACE_RENDERING
{
	class SpSceneRenderer
	{
	public:

		API_INTERFACE inline virtual void render(const SpSceneRendererData& rendererData)
		{
			SpCamera* camera = rendererData.scene->cameras()->get(rendererData.cameraIndex);
			SpViewportData* viewportData = rendererData.viewportData;

			camera->updateProjectionPerspectiveAspect(rendererData.viewportData->aspectRatio());

			rendererData.framebuffer->use();

			sp_opengl_check(glViewport(viewportData->x, viewportData->y, viewportData->width, viewportData->height));
			sp_opengl_check(glScissor(viewportData->x, viewportData->y, viewportData->width, viewportData->height));
			sp_opengl_check(glClearColor(1.0f, 0.0f, 0.0f, 1.0f));
			sp_opengl_check(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT));

			sp_opengl_check(glEnable(GL_LINE_SMOOTH));
			sp_opengl_check(glEnable(GL_DEPTH_TEST));
			sp_opengl_check(glEnable(GL_CULL_FACE));
			sp_opengl_check(glEnable(GL_POLYGON_OFFSET_FILL));
			sp_opengl_check(glPolygonOffset(1.0f, 1.0f));
			//sp_opengl_check(glPolygonMode(GL_FRONT_AND_BACK, GL_LINE));

			//render3D(renderData);

			//camera->getHUDProjectionMatrix((sp_float)_viewport.width, (sp_float)_viewport.height, renderData.projectionMatrix);

			sp_opengl_check(glDisable(GL_DEPTH_TEST));
			sp_opengl_check(glDisable(GL_CULL_FACE));

			//render2D(renderData);

			rendererData.framebuffer->disable();
		}

	};
}

#endif // SP_SCENE_RENDERER_HEADER