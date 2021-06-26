#ifndef SP_SCENE_RENDERER_DATA_HEADER
#define SP_SCENE_RENDERER_DATA_HEADER

#include "SpectrumRendering.h"
#include "SpScene.h"
#include "OpenGLFramebuffer.h"
#include "SpViewportData.h"

namespace NAMESPACE_RENDERING
{
	class SpSceneRendererData
	{
	public:
		SpScene* scene;
		sp_uint cameraIndex;
		OpenGLFramebuffer* framebuffer;
		SpViewportData* viewportData;

	};
}

#endif // SP_SCENE_RENDERER_DATA_HEADER