#ifndef CUBE_LIST
#define CUBE_LIST

#include "Cube.h"

namespace NAMESPACE_RENDERING
{
	class CubeList
		: public GraphicObject3D
	{
	private:
		CubeRenderer* renderer;

	public:
		Cube* cubes;
		size_t cubesSize;

		void init(CubeRenderer* renderer, Cube* cubes, size_t cubesSize);

		void render(const RenderData& renderData) override;

		API_INTERFACE const sp_char* toString() override
		{
			return "CubeList";
		}
	};
}

#endif // CUBE_LIST
