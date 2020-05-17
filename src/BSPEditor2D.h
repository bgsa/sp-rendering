#pragma once

#include "GraphicObject2D.h"
#include "BSPTree.h"

namespace NAMESPACE_RENDERING
{
	class BSPEditor2D : public GraphicObject2D
	{
	private:
		BSPTree * bspTree;

		void setTextureEnabled(BSPNode* node, bool isEnabled);

	public:
		API_INTERFACE BSPEditor2D(BSPTree * bspTree);

		API_INTERFACE void enableTexture();
		API_INTERFACE void disableTexture();

		BSPNode * findLeaf(Vec2 point);
		API_INTERFACE void removeLeaf(Vec2 point);

		API_INTERFACE void init();
		API_INTERFACE void render(const RenderData& renderData) override;

		API_INTERFACE const sp_char* toString()
		{
			return "BSP Editor 2D";
		}
	};
}