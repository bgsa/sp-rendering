#ifndef CUBE_HEADER
#define CUBE_HEADER

#include "SpectrumRendering.h"
#include <BoundingVolume.h>
#include "CubeRenderer.h"

namespace NAMESPACE_RENDERING
{
	class Cube
		: public GraphicObject3D<OBB>
	{
	private:
		CubeRenderer* renderer;

	public:

		API_INTERFACE void init(CubeRenderer* renderer);

		API_INTERFACE void render(const RenderData& renderData) override;

		API_INTERFACE const sp_char* toString() override
		{
			return "Cube";
		}

	};
}

#endif // CUBE_HEADER