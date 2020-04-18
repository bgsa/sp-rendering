#pragma once

#include "SpectrumRendering.h"
#include "BSPNode.h"
#include "BinaryTree.h"

namespace NAMESPACE_RENDERING
{
	class BSPTree : public BinaryTree<BSPNode>
	{
	public:

		std::string toString() {
			return "BSP Tree";
		}
	};
}