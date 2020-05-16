#ifndef I_GRAPHIC_OBJECT_RENDERER_HEADER
#define I_GRAPHIC_OBJECT_RENDERER_HEADER

#include "SpectrumRendering.h"
#include "RenderData.h"

namespace NAMESPACE_RENDERING
{
	class IGraphicObjectRenderer
		: public Object
	{
	public:

		API_INTERFACE virtual void update() = 0;

		API_INTERFACE virtual void render(const RenderData& renderData) = 0;

	};
}

#endif // I_GRAPHIC_OBJECT_RENDERER_HEADER