#include "Rock.h"

namespace NAMESPACE_RENDERING
{
	void Rock::render(const RenderData& renderData)
	{
		renderer->render(renderData);
	}

	void Rock::init()
	{
		ObjModel model;
		model.load("resources\\models\\rocks\\Rock 1.obj");
	}

}