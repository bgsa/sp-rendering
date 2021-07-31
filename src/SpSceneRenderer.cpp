#include "SpSceneRenderer.h"
#include "SpRenderingAPI.h"
#include "SpGame.h"

namespace NAMESPACE_RENDERING
{

	void SpSceneRenderer::render(const SpSceneRendererData& rendererData)
	{
		SpRenderingAPI* renderingApi = SpGameInstance->renderingAPI();
		const sp_uint typeFloatId = renderingApi->typeFloatId();
		const sp_uint typeUIntId = renderingApi->typeUIntId();
		const sp_uint typeTriangleId = renderingApi->typeTriangleId();

		SpCameraManager* cameraManager = rendererData.scene->camerasManager();
		SpCamera* camera = cameraManager->get(rendererData.cameraIndex);
		camera->update(rendererData.viewportData.aspectRatio());
		const Mat4 projectionMatrix = camera->projectionMatrix();
		const Mat4 viewMatrix = camera->viewMatrix();

		cameraManager->updateGpuBuffer();

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

			if (renderableObject->isVisible())
			{
				SpGameObject* gameObject = rendererData.scene->gameObject(renderableObject->gameObject());
				const SpMeshData* mesh = scene->meshManager()->get(renderableObject->meshData());
				SpShader* shader = scene->shaders[renderableObject->shader()];
				shader->enable();

				scene->camerasManager()->gpuBuffer()->use(0);
				shader->setUniform(0, 0);

				scene->transformManager()->gpuBuffer()->use(1);
				shader->setUniform(1, 1);

				scene->lightingManager()->gpuBuffer()->use(2);
				shader->setUniform(2, 2);

				scene->renderableObjectManager()->gpuBufferMaterials()->use(3);
				shader->setUniform(3, 3);

				shader->setUniform(4, (sp_int)scene->lightingManager()->length());
				shader->setUniform(5, rendererData.cameraIndex);
				shader->setUniform(6, renderableObject->gameObject());
				shader->setUniform(7, gameObject->renderableObjectIndex());

				// enable all buffers
				for (sp_int i = 0; i < renderableObject->buffersLength(); i++)
					renderableObject->buffers(i)->use();

				shader->enableVertexAttribute(0, 3, typeFloatId, false, sizeof(sp_float) * 6, 0);
				shader->enableVertexAttribute(1, 3, typeFloatId, false, sizeof(sp_float) * 6, (void*)(sizeof(sp_float) * 3));

				shader->drawElements(typeTriangleId, mesh->facesLength * 3, typeUIntId);

				// disable all buffers
				for (sp_int i = 0; i < renderableObject->buffersLength(); i++)
					renderableObject->buffers(i)->disable();

				scene->transformManager()->gpuBuffer()->disable();

				shader->disable();
			}
		}

		//camera->getHUDProjectionMatrix((sp_float)_viewport.width, (sp_float)_viewport.height, renderData.projectionMatrix);

		renderingApi->disable(GL_DEPTH_TEST);
		renderingApi->disable(GL_CULL_FACE);

		//render2D(renderData);

		rendererData.framebuffer->disable();
	}

}