#include "CubeList.h"

namespace NAMESPACE_RENDERING
{
	void CubeList::init(CubeRenderer* renderer, Cube* cubes, size_t cubesSize)
	{
		this->renderer = renderer;
		this->cubes = cubes;
		this->cubesSize = cubesSize;
	}

	void CubeList::render(const SpRenderData& renderData)
	{
		renderer->beforeRender(renderData, cubes, cubesSize);
		renderer->render(cubesSize);
	}
}