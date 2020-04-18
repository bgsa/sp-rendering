#pragma once

#include "BSPTree.h"
#include "GraphicObject3D.h"

namespace NAMESPACE_RENDERING
{
	class BSPEditor3D
		: public GraphicObject3D<AABB>
	{
	private:
		BSPTree * bspTree;

		void setTextureEnabled(BSPNode* node, bool isEnabled);

	public:
		BSPEditor3D(BSPTree* bspTree);

		void API_INTERFACE enableTexture();
		void API_INTERFACE disableTexture();

		void render(const RenderData& renderData) override;

		API_INTERFACE const sp_char* toString()
		{
			return "BSP Editor 3D";
		}
	};
}