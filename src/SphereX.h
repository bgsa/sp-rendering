#pragma once

#include "SpectrumRendering.h"
#include "GraphicObject3D.h"
#include "SphereRenderer.h"
#include <Sphere.h>

namespace NAMESPACE_RENDERING
{
	class SphereX
		: public GraphicObject3D
	{
	public:

		Sphere geometry;

		SphereRenderer* renderer;

		SphereX()
		{
			geometry = Sphere({ 0.0f, 0.0f, 0.0f }, 1.0f);
			renderer = NULL;
		}

		void update(long long elapsedTime) override
		{
		}

		void render(const RenderData& renderData) override
		{
			renderer->geometry = &geometry;
			renderer->setColor(*this->color);

			renderer->render(renderData);
		}

		const sp_char* toString() override
		{
			return "Sphere Game Object";
		}

	};
}
