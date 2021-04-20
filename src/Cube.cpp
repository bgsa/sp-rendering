#include <cstdlib>
#include <vector>

#include "Shader.h"
#include "Cube.h"

namespace NAMESPACE_RENDERING
{
	void Cube::init(CubeRenderer* renderer)
	{
		this->renderer = renderer;
	}

	void Cube::render(const SpRenderData& renderData)
	{
		renderer->beforeRender(renderData, this, 1);
		renderer->render(1);
	}
}