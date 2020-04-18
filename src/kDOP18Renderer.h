#ifndef K_DOP18_RENDERER
#define K_DOP18_RENDERER

#include "SpectrumRendering.h"
#include "GraphicObject3D.h"
#include "IRenderer.h"
#include "kDOP18.h"
#include <Plane3D.h>

namespace NAMESPACE_RENDERING
{
	class kDOP18Renderer
		: public IRenderer<kDOP18>
	{
	private:
		GLuint programShader;
		GLuint vertexBufferObject;
		GLuint modelBufferObject;

		GLint projectionMatrixLocation;
		GLint viewMatrixLocation;
		GLint modelMatrixLocation;

		GLint positionAttribute = -1;

		void initVBO();
		void setUpPositionAttribute();

		kDOP18* kdops = NULL;
		size_t kdopsCount = 0;

	public:

		API_INTERFACE void init();

		API_INTERFACE void setObjects(kDOP18* kdops, size_t count) override;

		API_INTERFACE void render(const RenderData& renderData) override;

		API_INTERFACE const sp_char* toString()
		{
			return "k-DOPs 18 Renderer";
		}

	};
}

#endif // !K_DOP18_RENDERER