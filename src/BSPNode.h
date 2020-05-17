#pragma once

#include "SpectrumRendering.h"
#include <ColorRGBA.h>
#include "GraphicObject2D.h"
//#include "Polygon2D.h"
#include "Polygon3D.h"
#include <Line2D.h>

#include <BinaryTreeNode.h>

namespace NAMESPACE_RENDERING
{
	class BSPNode 
		: public GraphicObject2D
		, public BinaryTreeNode<BSPNode*>
	{
	private:
		//static Polygon3D* calculatePolygon3D(Polygon2D* polygon2D, float height);

	public:
		NAMESPACE_PHYSICS::Line2D* line = nullptr;
		//Polygon2D* polygon = nullptr;
		Polygon3D* polygon3D = nullptr;
		float height = 100.0f;

		API_INTERFACE void init() override {}

		void split(NAMESPACE_PHYSICS::Line2D line);

		BSPNode* findLeaf(Vec2 point);
		void removeChild(BSPNode* child);

		static BSPNode* initRootNode();

		void render(const RenderData& renderData) override;
		//void render2D(RenderData renderData);
		void render3D(RenderData renderData);

		API_INTERFACE const sp_char* toString() {
			return "BSP TreeNode";
		}

		~BSPNode();
	};
}