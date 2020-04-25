#ifndef K_DOP_18_HEADER
#define K_DOP_18_HEADER

#include "SpectrumRendering.h"
#include <DOP18.h>
#include "IRenderer.h"
#include "GraphicObject3D.h"

namespace NAMESPACE_RENDERING
{
	class kDOP18
		: public GraphicObject3D<kDOP18, DOP18>
	{
	private:
		IRenderer<kDOP18>* renderer;

	public:

		API_INTERFACE kDOP18();

		API_INTERFACE void init(IRenderer<kDOP18>* renderer);

		API_INTERFACE void render(const RenderData& renderData) override;

		API_INTERFACE const sp_char* toString() override
		{
			return "k-DOP 18";
		}

	};
}

#endif // !K_DOP_18_HEADER