#ifndef SP_SCENE_RENDERER_DATA_HEADER
#define SP_SCENE_RENDERER_DATA_HEADER

#include "SpectrumRendering.h"
#include "SpScene.h"
#include "OpenGLFramebuffer.h"
#include "SpViewportData.h"
#include "SpColorRGBA.h"

namespace NAMESPACE_RENDERING
{
	class SpSceneRendererData
	{
	public:
		SpScene* scene;
		sp_uint cameraIndex;
		OpenGLFramebuffer* framebuffer;
		SpViewportData viewportData;

		SpColorRGBA backgroundColor;

		/// <summary>
		/// Default constructor
		/// </summary>
		/// <returns></returns>
		API_INTERFACE inline SpSceneRendererData()
		{
			backgroundColor = SpColorRGBABlack;
		}

	};
}

#endif // SP_SCENE_RENDERER_DATA_HEADER