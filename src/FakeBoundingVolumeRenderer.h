#ifndef FAKE_BOUNDING_VOLUME_RENDERER_HEADER
#define FAKE_BOUNDING_VOLUME_RENDERER_HEADER

#include "SpectrumRendering.h"
#include "BoundingVolumeRenderer.h"

namespace NAMESPACE_RENDERING
{
	class FakeBoundingVolumeRenderer
		: public BoundingVolumeRenderer
	{
	public:

		API_INTERFACE void update() override { }

		API_INTERFACE void render(const RenderData& renderData) override { }

		API_INTERFACE void dispose() override { }

		API_INTERFACE const sp_char* toString() override
		{
			return "FakeBoundingVolumeRenderer";
		}

	};
}

#endif // FAKE_BOUNDING_VOLUME_RENDERER_HEADER