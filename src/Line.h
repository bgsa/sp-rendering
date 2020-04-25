#ifndef LINE_HEADER
#define LINE_HEADER

#include "SpectrumRendering.h"
#include "GraphicObject3D.h"

namespace NAMESPACE_RENDERING
{
	class Line
		: public GraphicObject3D<Line, AABB>
	{
	private:
		float lineWidth = 3.0f;

		void initVBO();
		void setUpPositionAttribute();

	public:
		Vec3f point1;
		Vec3f point2;

		Line();
		Line(Vec3f point1, Vec3f point2);

		void init();

		void setLineWidth(float newLineWidth);

		void render(RenderData renderData);

		API_INTERFACE const sp_char* toString() {
			return "Line 3D";
		}
	};
}

#endif // !LINE_HEADER