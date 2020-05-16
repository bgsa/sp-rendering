#ifndef SPHERE_RENDERER_HEADER
#define SPHERE_RENDERER_HEADER

#include "SpectrumRendering.h"
#include "GraphicObject3D.h"
#include "Shader.h"
#include <Sphere.h>

namespace NAMESPACE_RENDERING
{
	class SphereRenderer
		: public GraphicObject3D
	{
	private:

		void initVBO();
		void setupAttributeLocations();

		GLint radiusLocation;

	public:

		Sphere* geometry;

		void init();

		void render(const RenderData& renderData);

		API_INTERFACE const sp_char* toString() override
		{
			return "Sphere";
		}

	};
}

#endif // !SPHERE_RENDERER_HEADER