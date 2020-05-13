#ifndef GRID_SYSTEM_HEADER
#define GRID_SYSTEM_HEADER

#include "SpectrumRendering.h"
#include "AABB.h"
#include "GraphicObject3D.h"

namespace NAMESPACE_RENDERING
{
	struct GridSystemShaderAttributes {
		GLfloat point1[3];
		GLfloat point2[3];
		GLfloat pointColor1[4];
		GLfloat pointColor2[4];
	};

	class GridSystem
		: public GraphicObject3D<GridSystem, AABB>
	{
	private:

		GridSystemShaderAttributes attributes;

		float lineWidth = 3.0f;

		void initVBO();
		void setUpPositionAttribute();

	public:
		void init();

		void setLineWidth(float newLineWidth);

		void render(const RenderData& renderData) override;

		API_INTERFACE const sp_char* toString() {
			return "GridSystem";
		}
	};
}

#endif // !GRID_SYSTEM_HEADER