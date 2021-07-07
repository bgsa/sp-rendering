#ifndef SP_SCENE_RENDERER_HEADER
#define SP_SCENE_RENDERER_HEADER

#include "SpectrumRendering.h"
#include "SpSceneRendererData.h"

namespace NAMESPACE_RENDERING
{
	class SpSceneRenderer
	{
	public:

		API_INTERFACE virtual void render(const SpSceneRendererData& rendererData);

	};
}

#endif // SP_SCENE_RENDERER_HEADER