#ifndef ROCK_RENDERER_HEADER
#define ROCK_RENDERER_HEADER

#include "SpectrumRendering.h"
#include "IGraphicObjectRenderer.h"
#include "Rock.h"

namespace NAMESPACE_RENDERING
{
	class RockRenderer
		: public IGraphicObjectRenderer<Rock>
	{
	public:

		API_INTERFACE void setObjects(Rock* rocks, sp_uint length);

		API_INTERFACE void update() override;

		API_INTERFACE void render(const RenderData& renderData) override;

		inline const sp_char* toString() override
		{
			return "RockRenderer";
		}
		
	};
}

#endif // ROCK_RENDERER_HEADER