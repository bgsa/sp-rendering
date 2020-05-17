#ifndef LINE_HEADER
#define LINE_HEADER

#include "SpectrumRendering.h"
#include "AABB.h"
#include "GraphicObject3D.h"

namespace NAMESPACE_RENDERING
{
	class Line
		: public GraphicObject3D
	{
	private:
		float lineWidth = 3.0f;

		void initVBO();
		void setUpPositionAttribute();

	public:
		Vec3 point1;
		Vec3 point2;

		Line();
		Line(Vec3 point1, Vec3 point2);

		void init();

		void setLineWidth(float newLineWidth);

		void render(RenderData renderData);

		API_INTERFACE const sp_char* toString() {
			return "Line 3D";
		}
	};
}

#endif // !LINE_HEADER