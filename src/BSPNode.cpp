#include "BSPNode.h"

#include <StringHelper.h>

namespace NAMESPACE_RENDERING
{
	void BSPNode::split(NAMESPACE_PHYSICS::Line2Df line)
	{
		/*
		TODO: REDO!

		this->line = new OpenML::Line2Df(line);

		OpenML::Rectangle2Df screenBound = RendererSettings::getInstance()->getScreenBound();
		Vec2f point1 = line.point1;
		Vec2f point2 = line.point2;

		if (point1 == point2)
			return;

		vector<OpenML::Line2Df> polygonLines = polygon->getLines();

		OpenML::Vec2f* intersectionPoint1 = nullptr;
		OpenML::Vec2f* intersectionPoint2 = nullptr;

		Polygon2D* polygon1 = new Polygon2D;
		Polygon2D* polygon2 = new Polygon2D;

		for (OpenML::Line2Df polygonLine : polygonLines)
		{
			Vec2f line2Point1 = polygonLine.point1;
			Vec2f line2Point2 = polygonLine.point2;

			float determinant = (line2Point2[0] - line2Point1[0]) * (point2[1] - point1[1]) - (line2Point2[1] - line2Point1[1]) * (point2[0] - point1[0]);

			if (determinant == 0.0f)
			{
				if (intersectionPoint1 == nullptr)
					polygon1->vertexes.push_back(polygonLine.point1);
				else
					if (intersectionPoint2 == nullptr)
						polygon2->vertexes.push_back(polygonLine.point2);
					else
						polygon1->vertexes.push_back(polygonLine.point1);

				continue;
			}

			float t = float(((line2Point2[0] - line2Point1[0]) * (line2Point1[1] - point1[1]) - (line2Point2[1] - line2Point1[1]) * (line2Point1[0] - point1[0])) / determinant);
			//float s = float(((point2.x - point1.x) * (line2Point1.y - point1.y) - (point2.y - point1.y) * (line2Point1.x - point1.x)) / determinant);

			if (intersectionPoint1 == nullptr)
			{
				intersectionPoint1 = new Vec2f(
					OpenML::round<float>(point1[0] + (point2[0] - point1[0]) * t, 3),
					OpenML::round<float>(point1[1] + (point2[1] - point1[1]) * t, 3)
				);

				OpenML::CollisionStatus status = screenBound.getSatusColision(*intersectionPoint1);
				bool isNotOnScreen = status == OpenML::CollisionStatus::OUTSIDE;
				bool isNotOnTheLine = !polygonLine.isOnTheLine(*intersectionPoint1);

				if (isNotOnScreen || isNotOnTheLine) {
					polygon1->vertexes.push_back(polygonLine.point1);
					intersectionPoint1 = nullptr;
					continue;
				}

				polygon2->vertexes.push_back(*intersectionPoint1);
				polygon2->vertexes.push_back(polygonLine.point2);

				polygon1->vertexes.push_back(polygonLine.point1);
				polygon1->vertexes.push_back(*intersectionPoint1);
			}
			else if (intersectionPoint2 == nullptr)
			{
				intersectionPoint2 = new Vec2f(
					OpenML::round<float>(point1[0] + (point2[0] - point1[0]) * t, 3),
					OpenML::round<float>(point1[1] + (point2[1] - point1[1]) * t, 3)
				);

				OpenML::CollisionStatus status = screenBound.getSatusColision(*intersectionPoint2);
				bool isNotOnScreen = status == OpenML::CollisionStatus::OUTSIDE;
				bool isNotOnTheLine = !polygonLine.isOnTheLine(*intersectionPoint2);

				if (isNotOnScreen || isNotOnTheLine) {
					polygon2->vertexes.push_back(polygonLine.point2);
					intersectionPoint2 = nullptr;
					continue;
				}

				polygon2->vertexes.push_back(*intersectionPoint2);
				polygon1->vertexes.push_back(*intersectionPoint2);
			}
			else
			{
				polygon1->vertexes.push_back(polygonLine.point1);
			}
		}

		ColorRGBAf* parentColor = polygon->getColor();
		float red = (float)(rand() % 255) / 255;
		float green = (float)(rand() % 255) / 255;
		float blue = (float)(rand() % 255) / 255;

		leftNode = new BSPNode;
		leftNode->parentNode = this;

		rightNode = new BSPNode;
		rightNode->parentNode = this;

		polygon1->setTexture(polygon->getTexture());
		polygon1->setTextureEnabled(polygon->isTextureEnabled());
		polygon2->setTexture(polygon->getTexture());
		polygon2->setTextureEnabled(polygon->isTextureEnabled());

		for (int i = 0; i < polygon1->vertexes.size(); i++)
		{
			Vec2f vertex = polygon1->vertexes[i];

			if (line.getOrientation(vertex) == OpenML::Orientation::NONE)
				continue;

			if (line.getOrientation(vertex) == OpenML::Orientation::LEFT)
			{
				leftNode->polygon = polygon1;
				leftNode->polygon->setColor(ColorRGBAf(parentColor->Red, parentColor->Green, parentColor->Blue, 1.0f));
				leftNode->polygon3D = calculatePolygon3D(polygon1, height);

				rightNode->polygon = polygon2;
				rightNode->polygon->setColor(ColorRGBAf(red, green, blue, 1.0f));
				rightNode->polygon3D = calculatePolygon3D(polygon2, height);
			}
			else
			{
				leftNode->polygon = polygon2;
				leftNode->polygon->setColor(ColorRGBAf(red, green, blue, 1.0f));
				leftNode->polygon3D = calculatePolygon3D(polygon2, height);

				rightNode->polygon = polygon1;
				rightNode->polygon->setColor(ColorRGBAf(parentColor->Red, parentColor->Green, parentColor->Blue, 1.0f));
				rightNode->polygon3D = calculatePolygon3D(polygon1, height);
			}

			break;
		}

		polygon1->init();
		polygon2->init();

		delete intersectionPoint1;
		delete intersectionPoint2;

		delete polygon;
		delete polygon3D;

		polygon = nullptr;
		polygon3D = nullptr;
		*/
	}

	BSPNode* BSPNode::findLeaf(Vec2f point)
	{
		if (this->line == nullptr)
			return this;

		NAMESPACE_PHYSICS::Orientation orientation = line->getOrientation(point);

		if (orientation == NAMESPACE_PHYSICS::Orientation::LEFT)
			return leftNode()->value->findLeaf(point);

		if (orientation == NAMESPACE_PHYSICS::Orientation::RIGHT)
			return rightNode()->value->findLeaf(point);

		return this;
	}

	void BSPNode::removeChild(BSPNode* child)
	{
		sp_assert(child == leftNode() || child == rightNode());

		/*
		TODO: REDO!

			if (leftNode->leftNode != nullptr || leftNode->rightNode != nullptr)
				return;

			if (rightNode->leftNode != nullptr || rightNode->rightNode != nullptr)
				return;

			this->polygon = new Polygon2D;

			if (child == leftNode)
			{
				for (Vec2f vertex : rightNode->polygon->vertexes)
					this->polygon->vertexes.push_back(vertex);

				this->polygon->setColor(*rightNode->polygon->getColor());
			}
			else
			{
				for (Vec2f vertex : leftNode->polygon->vertexes)
					this->polygon->vertexes.push_back(vertex);

				this->polygon->setColor(*leftNode->polygon->getColor());
			}

			this->polygon->init();

			delete leftNode;
			delete rightNode;

			leftNode = nullptr;
			rightNode = nullptr;
			*/
	}

	void BSPNode::render(const RenderData& renderData)
	{
		//render2D(renderData);
	}

	/*
	void BSPNode::render2D(RenderData renderData)
	{
		if (polygon != nullptr)
			polygon->render(renderData);

		if (leftNode() != nullptr)
			leftNode()->value->render2D(renderData);

		if (rightNode() != nullptr)
			rightNode()->value->render2D(renderData);
	}
	*/

	void BSPNode::render3D(RenderData renderData)
	{
		if (polygon3D != nullptr)
			polygon3D->render(renderData);

		if (leftNode() != nullptr)
			leftNode()->value->render3D(renderData);

		if (rightNode() != nullptr)
			rightNode()->value->render3D(renderData);
	}

	/*
	Polygon3D* BSPNode::calculatePolygon3D(Polygon2D* polygon2D, float height)
	{	
		ColorRGBAf* color = polygon2D->getColor();

		int vertexIndex = 0;
		int vertexesCount = (int) polygon2D->vertexes.size() * 2;

		Polygon3D* polygon3D = new Polygon3D;
		polygon3D->setColor({ color->Red, color->Green, color->Blue, color->Alpha });

		polygon3D->vertexes = Vec3List<float>(new Vec3f[vertexesCount], vertexesCount);

		for (Vec2f point2D : polygon2D->vertexes)
		{
			polygon3D->vertexes.points[vertexIndex] = { -point2D[0], height, point2D[1] };
			vertexIndex++;
		}

		for (Vec2f point2D : polygon2D->vertexes)
		{
			polygon3D->vertexes.points[vertexIndex] = { -point2D[0],   0.0f, point2D[1] };
			vertexIndex++;
		}

		size_t vertexCount = polygon3D->vertexes.count;
		size_t facesCount = vertexCount + (2 * (vertexCount / 2 - 2));

		polygon3D->facesSize = 3 * facesCount;
		polygon3D->faces = new GLubyte[3 * facesCount];

		size_t faceIndex = 0;

		//top face
		for (size_t i = 0; i < vertexCount / 2 - 2; i++)
		{
			polygon3D->faces[faceIndex] = (GLubyte)0;
			polygon3D->faces[faceIndex + 1] = (GLubyte)i + 1;
			polygon3D->faces[faceIndex + 2] = (GLubyte)i + 2;
			faceIndex += 3;
		}

		//bottom face
		for (size_t i = vertexCount - 1; i > vertexCount / 2 + 1; i--)
		{
			polygon3D->faces[faceIndex] = (GLubyte) vertexCount / 2;
			polygon3D->faces[faceIndex + 1] = (GLubyte) i;
			polygon3D->faces[faceIndex + 2] = (GLubyte) i - 1;
			faceIndex += 3;
		}

		//side faces
		for (size_t i = vertexCount / 2; i < vertexCount; i++)
		{
			polygon3D->faces[faceIndex] = (GLubyte)i;
			polygon3D->faces[faceIndex + 1] = (GLubyte)i + 1;
			polygon3D->faces[faceIndex + 2] = GLubyte(i - vertexCount / 2);

			polygon3D->faces[faceIndex + 3] = (GLubyte)i + 1;
			polygon3D->faces[faceIndex + 4] = GLubyte(i - vertexCount / 2 + 1);
			polygon3D->faces[faceIndex + 5] = GLubyte(i - vertexCount / 2);

			faceIndex += 6;
		}
		polygon3D->faces[faceIndex - 5] = (GLubyte)0;
		polygon3D->faces[faceIndex - 3] = (GLubyte)vertexCount - 1;
		polygon3D->faces[faceIndex - 1] = (GLubyte)0;

		//for (size_t i = 0; i < 3 * facesCount; i += 3)
			//Log::info(StringHelper::toString((size_t)polygon3D->faces[i]) + " , " + StringHelper::toString((size_t)polygon3D->faces[i + 1]) + " , " + StringHelper::toString((size_t)polygon3D->faces[i + 2]));

		polygon3D->setTexture(polygon2D->getTexture());
		polygon3D->setTextureEnabled(polygon2D->isTextureEnabled());
		polygon3D->init();
		polygon3D->scaleUniform(0.02f);
		polygon3D->translate(400.0f, 0.0f, -300.0f);

		return polygon3D;
		return nullptr;
	}
			*/

	BSPNode* BSPNode::initRootNode()
	{
		/*
		TODO: REDO!

		BSPNode* root = new BSPNode;
		root->polygon = new Polygon2D;
		root->polygon->setTexture("resources/images/textura1.png");
		root->polygon->setTextureEnabled(false);
		root->polygon->setColor({ 0.0f, 0.2f, 1.0f, 1.0f });

		float width = (float)RendererSettings::getInstance()->getWidth();
		float height = (float)RendererSettings::getInstance()->getHeight();

		Vec2f point1 = { 0.0f, 0.0f };
		root->polygon->vertexes.push_back(point1);

		Vec2f point2 = { width, 0.0f };
		root->polygon->vertexes.push_back(point2);

		Vec2f point3 = { width, height };
		root->polygon->vertexes.push_back(point3);

		Vec2f point4 = { 0.0f, height };
		root->polygon->vertexes.push_back(point4);

		root->polygon3D = calculatePolygon3D(root->polygon, 100.0f);

		root->polygon->init();

		return root;
		*/

		return nullptr;
	}

	BSPNode::~BSPNode()
	{
		//if (polygon != nullptr)
		//	delete polygon;

		if (polygon3D != nullptr)
			delete polygon3D;

		if (line != nullptr)
			delete line;
	}
}