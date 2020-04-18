#include "kDOP18.h"

namespace NAMESPACE_RENDERING
{
	kDOP18::kDOP18()
	{
		boundingVolume = ALLOC_NEW(DOP18)();
	}

	void kDOP18::init(IRenderer<kDOP18>* renderer)
	{
		this->renderer = renderer;
	}

	void kDOP18::render(const RenderData& renderData)
	{
		renderer->render(renderData);
	}
}