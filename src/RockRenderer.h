#ifndef ROCK_RENDERER_HEADER
#define ROCK_RENDERER_HEADER

#include "SpectrumRendering.h"
#include "IRenderer.h"
#include "Rock.h"

namespace NAMESPACE_RENDERING
{
	class RockRenderer
		: public IRenderer<Rock>
	{
	public:

		API_INTERFACE void setObjects(Rock* rocks, sp_uint length) override;

		API_INTERFACE void update() override;

		API_INTERFACE void render(const RenderData& renderData) override;

		inline const sp_char* toString() override
		{
			return "RockRenderer";
		}
		
	};
}

#endif // ROCK_RENDERER_HEADER