#ifndef GRID_SYSTEM_HEADER
#define GRID_SYSTEM_HEADER

#include "SpectrumRendering.h"
#include "AABB.h"
#include "GraphicObject3D.h"

namespace NAMESPACE_RENDERING
{
	struct GridSystemShaderAttributes {
		sp_float point1[3];
		sp_float point2[3];
		sp_float pointColor1[4];
		sp_float pointColor2[4];
	};

	class GridSystem
		: public GraphicObject3D
	{
	private:

		GridSystemShaderAttributes attributes;

		sp_float lineWidth = 1.0f;

		void initVBO();
		void setUpPositionAttribute();

	public:
		API_INTERFACE void init();

		API_INTERFACE void setLineWidth(float newLineWidth);

		API_INTERFACE void render(const RenderData& renderData) override;

		API_INTERFACE const sp_char* toString() { return "GridSystem"; }
	};
}

#endif // !GRID_SYSTEM_HEADER