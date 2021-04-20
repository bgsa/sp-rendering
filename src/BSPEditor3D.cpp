#include "BSPEditor3D.h"

namespace NAMESPACE_RENDERING
{
	BSPEditor3D::BSPEditor3D(BSPTree* bspTree)
	{
		this->bspTree = bspTree;
	}

	void BSPEditor3D::render(const SpRenderData& renderData)
	{
		bspTree->root->value.render3D(renderData);
	}

	void BSPEditor3D::setTextureEnabled(BSPNode* node, bool isEnabled)
	{
		if (node->polygon3D != nullptr)
		{
			node->polygon3D->setTextureEnabled(isEnabled);
//			node->polygon->setTextureEnabled(isEnabled);
			return;
		}

		setTextureEnabled(node->leftNode()->value, isEnabled);
		setTextureEnabled(node->rightNode()->value, isEnabled);
	}

	void BSPEditor3D::enableTexture()
	{
		setTextureEnabled(&bspTree->root->value, true);
	}

	void BSPEditor3D::disableTexture()
	{
		setTextureEnabled(&bspTree->root->value, false);
	}
}