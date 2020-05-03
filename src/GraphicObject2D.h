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
	private:

	protected:
		ColorRGBAf* color = new ColorRGBAf{ 1.0f, 1.0f, 1.0f, 1.0f };
		ColorRGBAf* borderColor = new ColorRGBAf{ 1.0f, 0.0f, 1.0f, 1.0f };
		float borderWidth = 0.0f;
		GraphicObject2D* parentObject2D = nullptr;
		std::vector<GraphicObject2D*> childrenObject2D;
		ContentAlignment alignment;
		Vec2f margin;

	public:

		GraphicObjectType type() override
		{
			return GraphicObjectType::Type2D;
		}

		virtual GraphicObject2D* getParent() {
			return parentObject2D;
		}
		virtual void setParent(GraphicObject2D* parentObject) {
			parentObject2D = parentObject;
		}

		virtual Vec2f getParentPosition()
		{
			Vec2f parentPosition = { 0.0f , 0.0f };

			if (parentObject2D != nullptr)
				parentPosition = parentObject2D->getPosition();

			return parentPosition;
		}
		virtual Vec2f getParentSize()
		{
			float parentWidth = float(RendererSettings::getInstance()->getWidth());
			float parentHeight = float(RendererSettings::getInstance()->getHeight());

			if (parentObject2D != nullptr) {
				parentWidth = parentObject2D->getWidth();
				parentHeight = parentObject2D->getHeight();
			}

			return Vec2f(parentWidth, parentHeight);
		}

		virtual ColorRGBAf* getColor() {
			return color;
		}
		virtual void setColor(ColorRGBAf color) {
			this->color->Red = color.Red;
			this->color->Green = color.Green;
			this->color->Blue = color.Blue;
			this->color->Alpha = color.Alpha;
		}

		virtual Vec2f getPosition()
		{
			return { modelView[12], modelView[13] };
		}
		virtual void setPosition(Vec2f position)
		{
			modelView[12] = position[1];
			modelView[13] = position[1];
		}

		virtual void setBotomMargin(float bottomMargin)
		{
			sp_assert(bottomMargin > 0.0f);
			margin[1] = bottomMargin;
		}
		virtual void setLeftMargin(float leftMargin)
		{
			sp_assert(leftMargin > 0.0f);
			margin[0] = leftMargin;
		}

		virtual void setRelativeBottomMargin(float relativeBottomMargin)
		{
			sp_assert(relativeBottomMargin > 0.0f);

			Vec2f parentSize = getParentSize();

			margin[1] = parentSize[1] * (relativeBottomMargin / 100.0f);
		}
		virtual void setRelativeLeftMargin(float relativeLeftMargin)
		{
			sp_assert(relativeLeftMargin > 0.0f);

			Vec2f parentSize = getParentSize();

			margin[0] = parentSize[0] * (relativeLeftMargin / 100.0f);
		}

		virtual ContentAlignment getAlignment()
		{
			return alignment;
		}
		virtual void setAlignment(ContentAlignment alignment)
		{
			this->alignment = alignment;
		}

		virtual void verticalAlignToCenter()
		{
			alignment.vertical = VerticalAlignment::CENTER;

			Vec2f parentSize = getParentSize();
			Vec2f parentPosition = getParentPosition();

			float leftMargin = parentPosition[0] + (parentSize[0] / 2.0f) - (getWidth() / 2.0f);

			modelView[12] = leftMargin;
		}
		virtual void horizontalAlignToCenter()
		{
			alignment.horizontal = HorizontalAlignment::MIDDLE;

			Vec2f parentSize = getParentSize();
			Vec2f parentPosition = getParentPosition();

			float bottomMargin = parentPosition[1] + (parentSize[1] / 2.0f) - (getHeight() / 2.0f);

			modelView[13] = bottomMargin;
		}

		virtual float getWidth()
		{
			return modelView[0];
		}
		virtual void setWidth(float width)
		{
			modelView[0] = width;
		}
		virtual void setRelativeWidth(float relativeWidth)
		{
			sp_assert(relativeWidth > 0);

			float windowWidth = float(RendererSettings::getInstance()->getWidth());
			modelView[0] = windowWidth * (relativeWidth / 100.0f);
		}

		virtual float getHeight()
		{
			return modelView[5];
		}
		virtual void setHeight(float height)
		{
			modelView[5] = height;
		}
		virtual void setRelativeHeight(float relativeHeight)
		{
			sp_assert(relativeHeight > 0);

			float windowHeight = float(RendererSettings::getInstance()->getHeight());
			modelView[5] = windowHeight * (relativeHeight / 100.0f);
		}

		virtual void setSize(float width, float height)
		{
			setWidth(width);
			setHeight(height);
		}

		virtual float getBorderWidth()
		{
			return borderWidth;
		}
		virtual void setBorderWidth(float borderWidth)
		{
			this->borderWidth = borderWidth;
		}

		virtual void setBorderColor(ColorRGBAf color)
		{
			borderColor->Red = color.Red;
			borderColor->Green = color.Green;
			borderColor->Blue = color.Blue;
			borderColor->Alpha = color.Alpha;
		}
		virtual ColorRGBAf* getBorderColor()
		{
			return borderColor;
		}

		virtual void renderBorder(RenderData renderData, size_t pointsCount)
		{
			if (borderWidth == 0.0f)
				return;

			glUniform4f(colorLocation, borderColor->Red, borderColor->Green, borderColor->Blue, borderColor->Alpha);

			float currentLineWidth = GLConfig::getGLfloat(GL_LINE_WIDTH);
			glLineWidth(borderWidth);

			glDrawArrays(GL_LINE_LOOP, 0, pointsCount);

			glLineWidth(currentLineWidth);
		}

		virtual ~GraphicObject2D() {
			delete color;
			delete borderColor;

			for (size_t i = 0; i < childrenObject2D.size(); i++)
				delete childrenObject2D[i];
		}

	};
}

#endif // GRAPHICS_OBJECT_2D