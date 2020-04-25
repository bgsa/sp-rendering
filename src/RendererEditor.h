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
	protected:
		IRendererManager* renderer = NULL;

	public:

		API_INTERFACE virtual void init(IRendererManager* renderer) = 0;

		API_INTERFACE virtual void update() = 0;

		API_INTERFACE virtual void preRender() = 0;

		API_INTERFACE virtual void render(const RenderData& renderData) = 0;

		API_INTERFACE virtual void postRender() = 0;

	};
}

#endif // RENDERER_EDITOR_HEADER