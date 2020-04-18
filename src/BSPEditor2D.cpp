#include "BSPEditor2D.h"

namespace NAMESPACE_RENDERING
{
	BSPEditor2D::BSPEditor2D(BSPTree * bspTree)
	{
		this->bspTree = bspTree;
	}

	BSPNode* BSPEditor2D::findLeaf(Vec2f point)
	{
		if (bspTree->root == nullptr)
			return nullptr;

		return bspTree->root->value.findLeaf(point);
	}

	void BSPEditor2D::removeLeaf(Vec2f point)
	{
		if (bspTree->root == nullptr)
			return;

		BSPNode* node = bspTree->root->value.findLeaf(point);

		if (node == nullptr || node == &bspTree->root->value)
			return;

		node->parentNode()->value->removeChild(node);
	}

	void BSPEditor2D::init()
	{
		bspTree->root->value = *BSPNode::initRootNode();
	}

	void BSPEditor2D::render(const RenderData& renderData)
	{
		//bspTree->root->value.render2D(renderData);
	}

	void BSPEditor2D::setTextureEnabled(BSPNode* node, bool isEnabled)
	{
		/*
		if (node->polygon != nullptr)
		{
			node->polygon->setTextureEnabled(isEnabled);
			node->polygon3D->setTextureEnabled(isEnabled);
			return;
		}

		setTextureEnabled((node->leftNode()->value), isEnabled);
		setTextureEnabled(node->rightNode()->value, isEnabled);
		*/
	}

	void BSPEditor2D::enableTexture()
	{
		setTextureEnabled(&bspTree->root->value, true);
	}

	void BSPEditor2D::disableTexture()
	{
		setTextureEnabled(&bspTree->root->value, false);
	}
}
