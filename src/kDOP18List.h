#ifndef K_DOP_18_HEADER
#define K_DOP_18_HEADER

#include "SpectrumRendering.h"
#include <DOP18.h>
#include "GraphicObject3DList.h"

namespace NAMESPACE_RENDERING
{
	class kDOP18List
		: public GraphicObject3DList
	{
	private:
		void updateTransformations() const;

	public:

		API_INTERFACE kDOP18List();

		API_INTERFACE void init() override;

		API_INTERFACE void render(const RenderData& renderData) override;

		API_INTERFACE void dispose() override;

		API_INTERFACE const sp_char* toString() override
		{
			return "k-DOP 18";
		}

	};
}

#endif // K_DOP_18_HEADER