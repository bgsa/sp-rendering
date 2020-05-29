#ifndef RENDERER_EDITOR_HEADER
#define RENDERER_EDITOR_HEADER

#include "SpectrumPhysics.h"
#include "IRendererManager.h"
#include "RenderData.h"

namespace NAMESPACE_RENDERING
{
	class RendererEditor
		: public Object
	{
	public:

		API_INTERFACE virtual void init() = 0;

		API_INTERFACE virtual void update(sp_float elapsedTime) = 0;

		API_INTERFACE virtual void preRender() = 0;

		API_INTERFACE virtual void render() = 0;

		API_INTERFACE virtual void renderGUI() = 0;

		API_INTERFACE virtual void postRender() = 0;

	};
}

#endif // RENDERER_EDITOR_HEADER