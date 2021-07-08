#include "SpSceneRenderer.h"
#include "SpRenderingAPI.h"
#include "SpGame.h"

namespace NAMESPACE_RENDERING
{

	void SpSceneRenderer::render(const SpSceneRendererData& rendererData)
	{
		SpRenderingAPI* renderingApi = SpGameInstance->renderingAPI();
		const sp_int typeFloatId = renderingApi->typeFloatId();
		const sp_int typeUIntId = renderingApi->typeUIntId();
		const sp_int typeTriangleId = renderingApi->typeTriangleId();

		SpCamera* camera = rendererData.scene->cameras()->get(rendererData.cameraIndex);
		camera->updateProjectionPerspectiveAspect(rendererData.viewportData.aspectRatio());
		const Mat4 projectionMatrix = camera->getProjectionMatrix();
		const Mat4 viewMatrix = camera->getViewMatrix();

		rendererData.framebuffer->use();

		renderingApi->setViewport(rendererData.viewportData);
		renderingApi->setScissor(SpRect<sp_int>(rendererData.viewportData.x, rendererData.viewportData.y, rendererData.viewportData.width, rendererData.viewportData.height));
		renderingApi->clearColor(rendererData.backgroundColor);
		renderingApi->clearFramebuffer();

		renderingApi->enableLineSmooth();
		renderingApi->enableDepthTest();
		renderingApi->enableCullingFace();
		renderingApi->enablePolygonOffsetFill();
		renderingApi->setPolygonOffset(1.0f, 1.0f);
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
			shader->setUniform(2, renderableObject->gameObjectIndex);

			scene->transformManager()->gpuBuffer()->use();

			// enable all buffers
			for (SpVectorItem<SpGpuBuffer*>* bufferItem = renderableObject->buffers.begin(); bufferItem != nullptr; bufferItem = bufferItem->next())
				bufferItem->value()->use();
				
			shader->enableVertexAttribute(0, 3, typeFloatId, false, sizeof(sp_float) * 6, 0);
			shader->enableVertexAttribute(1, 3, typeFloatId, false, sizeof(sp_float) * 6, (void*)(sizeof(sp_float) * 3));

			shader->drawElements(typeTriangleId, mesh->facesLength * 3, typeUIntId);

			// disable all buffers
			for (SpVectorItem<SpGpuBuffer*>* bufferItem = renderableObject->buffers.begin(); bufferItem != nullptr; bufferItem = bufferItem->next())
				bufferItem->value()->disable();

			scene->transformManager()->gpuBuffer()->disable();

			shader->disable();
		}

		//camera->getHUDProjectionMatrix((sp_float)_viewport.width, (sp_float)_viewport.height, renderData.projectionMatrix);

		renderingApi->disable(GL_DEPTH_TEST);
		renderingApi->disable(GL_CULL_FACE);

		//render2D(renderData);

		rendererData.framebuffer->disable();
	}

}