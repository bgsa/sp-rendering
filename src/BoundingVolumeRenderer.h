#ifndef BOUNDING_VOLUME_RENDERER_HEADER
#define BOUNDING_VOLUME_RENDERER_HEADER

#include "SpectrumRendering.h"
#include "IGraphicObjectRenderer.h"
#include "BoundingVolume.h"

namespace NAMESPACE_RENDERING
{
	class BoundingVolumeRenderer
		: public IGraphicObjectRenderer
	{
	private:

		void render(const RenderData& renderData) override { };

	public:

		API_INTERFACE virtual void render(const RenderData& renderData, BoundingVolume* boundingVolume) = 0;

	};
}

#endif // BOUNDING_VOLUME_RENDERER_HEADER