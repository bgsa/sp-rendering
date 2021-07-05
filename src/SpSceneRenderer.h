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
			SpRenderingFactory* renderingApi = SpGameInstance->renderingFactory();
			const sp_int typeFloatId = renderingApi->typeFloatId();
			const sp_int typeUIntId = renderingApi->typeUIntId();
			const sp_int typeTriangleId = renderingApi->typeTriangleId();

			SpCamera* camera = rendererData.scene->cameras()->get(rendererData.cameraIndex);
			camera->updateProjectionPerspectiveAspect(rendererData.viewportData.aspectRatio());
			const Mat4 projectionMatrix = camera->getProjectionMatrix();
			const Mat4 viewMatrix = camera->getViewMatrix();

			rendererData.framebuffer->use();

			sp_opengl_check(glViewport(rendererData.viewportData.x, rendererData.viewportData.y, rendererData.viewportData.width, rendererData.viewportData.height));
			sp_opengl_check(glScissor(rendererData.viewportData.x, rendererData.viewportData.y, rendererData.viewportData.width, rendererData.viewportData.height));
			sp_opengl_check(glClearColor(rendererData.backgroundColor.red, rendererData.backgroundColor.green, rendererData.backgroundColor.blue, rendererData.backgroundColor.alpha));
			sp_opengl_check(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT));

			sp_opengl_check(glEnable(GL_LINE_SMOOTH));
			sp_opengl_check(glEnable(GL_DEPTH_TEST));
			sp_opengl_check(glEnable(GL_CULL_FACE));
			sp_opengl_check(glEnable(GL_POLYGON_OFFSET_FILL));
			sp_opengl_check(glPolygonOffset(1.0f, 1.0f));
			//sp_opengl_check(glPolygonMode(GL_FRONT_AND_BACK, GL_LINE));

			SpScene* scene = rendererData.scene;
			SpRenderableObjectManager* renderableObjectManager = scene->renderableObjectManager();
			
			// render all objects
			for (sp_uint i = 0; i < renderableObjectManager->length(); i++)
			{
				SpRenderableObject* renderableObject = renderableObjectManager->get(i);
				const SpMeshData* mesh = scene->meshManager()->get(renderableObject->meshDataIndex);
				SpShader* shader = scene->shaders[renderableObject->shaderIndex];
				shader->enable();

				shader->setUniform(0, projectionMatrix);
				shader->setUniform(1, viewMatrix);
				//shader->setUniform<sp_size>(2, gameobjectIndex);

				// enable all buffers
				for (SpVectorItem<SpGpuBuffer*>* bufferItem = renderableObject->buffers.begin(); bufferItem != nullptr; bufferItem = bufferItem->next())
					bufferItem->value()->use();

				shader->enableVertexAttribute(0, 3, typeFloatId, false, 0, 0);
				shader->enableVertexAttribute(1, 3, typeFloatId, false, 3, 0);

				shader->drawElements(typeTriangleId, mesh->facesLength * 3, typeUIntId);

				// disable all buffers
				for (SpVectorItem<SpGpuBuffer*>* bufferItem = renderableObject->buffers.begin(); bufferItem != nullptr; bufferItem = bufferItem->next())
					bufferItem->value()->disable();

				shader->disable();
			}

			//camera->getHUDProjectionMatrix((sp_float)_viewport.width, (sp_float)_viewport.height, renderData.projectionMatrix);

			sp_opengl_check(glDisable(GL_DEPTH_TEST));
			sp_opengl_check(glDisable(GL_CULL_FACE));

			//render2D(renderData);

			rendererData.framebuffer->disable();
		}

	};
}

#endif // SP_SCENE_RENDERER_HEADER