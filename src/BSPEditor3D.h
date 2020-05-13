#pragma once

#include "BSPTree.h"
#include "AABB.h"
#include "GraphicObject3D.h"

namespace NAMESPACE_RENDERING
{
	class BSPEditor3D
		: public GraphicObject3D<BSPEditor3D, AABB>
	{
	private:
		BSPTree * bspTree;

		void setTextureEnabled(BSPNode* node, bool isEnabled);

	public:
		BSPEditor3D(BSPTree* bspTree);

		API_INTERFACE void init() override {}

		API_INTERFACE void enableTexture();
		API_INTERFACE void disableTexture();

		API_INTERFACE void render(const RenderData& renderData) override;

		API_INTERFACE const sp_char* toString()
		{
			return "BSP Editor 3D";
		}
	};
}