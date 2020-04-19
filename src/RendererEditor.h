#ifndef RENDERER_EDITOR_HEADER
#define RENDERER_EDITOR_HEADER

#include "SpectrumPhysics.h"
#include "RenderData.h"

namespace NAMESPACE_RENDERING
{
	class RendererEditor
		: public Object
	{
	public:

		API_INTERFACE virtual void init() = 0;

		API_INTERFACE virtual void update() = 0;

		API_INTERFACE virtual void render(const RenderData& renderData) = 0;

	};
}

#endif // RENDERER_EDITOR_HEADER