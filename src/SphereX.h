#pragma once

#include "SpectrumRendering.h"
#include "GraphicObject3D.h"
#include "SphereRenderer.h"
#include <Sphere.h>

namespace NAMESPACE_RENDERING
{
	class SphereX
		: public GraphicObject3D<SphereX, NAMESPACE_PHYSICS::Sphere>
	{
	public:

		NAMESPACE_PHYSICS::Sphere geometry;

		SphereRenderer* renderer;

		SphereX()
		{
			geometry = NAMESPACE_PHYSICS::Sphere({ 0.0f, 0.0f, 0.0f }, 1.0f);
			renderer = NULL;
		}

		void update(long long elapsedTime) override
		{
			geometry.particleSystem->update(elapsedTime);
			geometry.center = geometry.particleSystem->particles[0].position;

			Vec3f position = geometry.particleSystem->particles[0].position;

			transform = Mat4f::createTranslate(position.x, position.y, position.z);
		}

		void render(const RenderData& renderData) override
		{
			renderer->geometry = &geometry;
			renderer->transform = this->transform;
			renderer->setColor(*this->color);

			renderer->render(renderData);
		}

		const sp_char* toString() override
		{
			return "Sphere Game Object";
		}

	};
}
