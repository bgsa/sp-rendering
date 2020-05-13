#ifndef GRAPHICS_OBJECT_2D
#define GRAPHICS_OBJECT_2D

#include "SpectrumRendering.h"
#include "GLConfig.h"
#include "GraphicObject.h"
#include "RendererSettings.h"
#include <vector>
#include "ContentAlignment.h"
#include <ColorRGBA.h>

namespace NAMESPACE_RENDERING
{
	class GraphicObject2D : public GraphicObject
	{
	protected:
		ColorRGBAf* color = new ColorRGBAf{ 1.0f, 1.0f, 1.0f, 1.0f };
		ColorRGBAf* borderColor = new ColorRGBAf{ 1.0f, 0.0f, 1.0f, 1.0f };
		sp_float borderWidth = 0.0f;
		GraphicObject2D* parentObject2D = nullptr;
		std::vector<GraphicObject2D*> childrenObject2D;
		ContentAlignment alignment;
		Vec2f margin;

	public:

		API_INTERFACE GraphicObjectType type() override
		{
			return GraphicObjectType::Type2D;
		}

		API_INTERFACE virtual GraphicObject2D* getParent() {
			return parentObject2D;
		}
		API_INTERFACE virtual void setParent(GraphicObject2D* parentObject) {
			parentObject2D = parentObject;
		}

		API_INTERFACE virtual Vec2f getParentPosition()
		{
			Vec2f parentPosition = { 0.0f , 0.0f };

			if (parentObject2D != nullptr)
				parentPosition = parentObject2D->getPosition();

			return parentPosition;
		}
		API_INTERFACE virtual Vec2f getParentSize()
		{
			sp_float parentWidth = sp_float(RendererSettings::getInstance()->getWidth());
			sp_float parentHeight = sp_float(RendererSettings::getInstance()->getHeight());

			if (parentObject2D != nullptr) {
				parentWidth = parentObject2D->getWidth();
				parentHeight = parentObject2D->getHeight();
			}

			return Vec2f(parentWidth, parentHeight);
		}

		API_INTERFACE virtual ColorRGBAf* getColor() {
			return color;
		}
		API_INTERFACE virtual void setColor(ColorRGBAf color) {
			this->color->Red = color.Red;
			this->color->Green = color.Green;
			this->color->Blue = color.Blue;
			this->color->Alpha = color.Alpha;
		}

		API_INTERFACE virtual Vec2f getPosition()
		{
			return { transform[12], transform[13] };
		}
		API_INTERFACE virtual void setPosition(Vec2f position)
		{
			transform[12] = position[1];
			transform[13] = position[1];
		}

		API_INTERFACE virtual void setBotomMargin(sp_float bottomMargin)
		{
			sp_assert(bottomMargin > 0.0f);
			margin[1] = bottomMargin;
		}
		API_INTERFACE virtual void setLeftMargin(sp_float leftMargin)
		{
			sp_assert(leftMargin > 0.0f);
			margin[0] = leftMargin;
		}

		API_INTERFACE virtual void setRelativeBottomMargin(sp_float relativeBottomMargin)
		{
			sp_assert(relativeBottomMargin > 0.0f);

			Vec2f parentSize = getParentSize();

			margin[1] = parentSize[1] * (relativeBottomMargin / 100.0f);
		}
		API_INTERFACE virtual void setRelativeLeftMargin(sp_float relativeLeftMargin)
		{
			sp_assert(relativeLeftMargin > 0.0f);

			Vec2f parentSize = getParentSize();

			margin[0] = parentSize[0] * (relativeLeftMargin / 100.0f);
		}

		API_INTERFACE virtual ContentAlignment getAlignment()
		{
			return alignment;
		}
		API_INTERFACE virtual void setAlignment(ContentAlignment alignment)
		{
			this->alignment = alignment;
		}

		API_INTERFACE virtual void verticalAlignToCenter()
		{
			alignment.vertical = VerticalAlignment::CENTER;

			Vec2f parentSize = getParentSize();
			Vec2f parentPosition = getParentPosition();

			float leftMargin = parentPosition[0] + (parentSize[0] / 2.0f) - (getWidth() / 2.0f);

			transform[12] = leftMargin;
		}
		API_INTERFACE virtual void horizontalAlignToCenter()
		{
			alignment.horizontal = HorizontalAlignment::MIDDLE;

			Vec2f parentSize = getParentSize();
			Vec2f parentPosition = getParentPosition();

			sp_float bottomMargin = parentPosition[1] + (parentSize[1] / 2.0f) - (getHeight() / 2.0f);

			transform[13] = bottomMargin;
		}

		API_INTERFACE virtual sp_float getWidth()
		{
			return transform[0];
		}
		API_INTERFACE virtual void setWidth(sp_float width)
		{
			transform[0] = width;
		}
		API_INTERFACE virtual void setRelativeWidth(sp_float relativeWidth)
		{
			sp_assert(relativeWidth > 0);

			float windowWidth = sp_float(RendererSettings::getInstance()->getWidth());
			transform[0] = windowWidth * (relativeWidth / 100.0f);
		}

		API_INTERFACE virtual sp_float getHeight()
		{
			return transform[5];
		}
		API_INTERFACE virtual void setHeight(sp_float height)
		{
			transform[5] = height;
		}
		API_INTERFACE virtual void setRelativeHeight(sp_float relativeHeight)
		{
			sp_assert(relativeHeight > 0);

			sp_float windowHeight = sp_float(RendererSettings::getInstance()->getHeight());
			transform[5] = windowHeight * (relativeHeight / 100.0f);
		}

		API_INTERFACE virtual void setSize(sp_float width, sp_float height)
		{
			setWidth(width);
			setHeight(height);
		}

		API_INTERFACE virtual float getBorderWidth()
		{
			return borderWidth;
		}
		API_INTERFACE virtual void setBorderWidth(sp_float borderWidth)
		{
			this->borderWidth = borderWidth;
		}

		API_INTERFACE virtual void setBorderColor(ColorRGBAf color)
		{
			borderColor->Red = color.Red;
			borderColor->Green = color.Green;
			borderColor->Blue = color.Blue;
			borderColor->Alpha = color.Alpha;
		}
		API_INTERFACE virtual ColorRGBAf* getBorderColor()
		{
			return borderColor;
		}

		API_INTERFACE virtual void renderBorder(RenderData renderData, sp_size pointsCount)
		{
			if (borderWidth == 0.0f)
				return;

			glUniform4f(colorLocation, borderColor->Red, borderColor->Green, borderColor->Blue, borderColor->Alpha);

			sp_float currentLineWidth = GLConfig::getGLfloat(GL_LINE_WIDTH);
			glLineWidth(borderWidth);

			glDrawArrays(GL_LINE_LOOP, 0, pointsCount);

			glLineWidth(currentLineWidth);
		}

		API_INTERFACE virtual ~GraphicObject2D() {
			delete color;
			delete borderColor;

			for (sp_size i = 0; i < childrenObject2D.size(); i++)
				delete childrenObject2D[i];
		}

	};
}

#endif // GRAPHICS_OBJECT_2D