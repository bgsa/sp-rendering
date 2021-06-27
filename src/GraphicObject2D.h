#ifndef GRAPHICS_OBJECT_2D
#define GRAPHICS_OBJECT_2D

#include <vector>
#include "SpectrumRendering.h"
#include "GraphicObject.h"
#include "GLConfig.h"
#include "ContentAlignment.h"
#include <SpColorRGBA.h>

namespace NAMESPACE_RENDERING
{
	class GraphicObject2D : public GraphicObject
	{
	protected:
		SpColorRGBA* color = new SpColorRGBA{ 1.0f, 1.0f, 1.0f, 1.0f };
		SpColorRGBA* borderColor = new SpColorRGBA{ 1.0f, 0.0f, 1.0f, 1.0f };
		sp_float borderWidth = 0.0f;
		GraphicObject2D* parentObject2D = nullptr;
		std::vector<GraphicObject2D*> childrenObject2D;
		ContentAlignment alignment;
		Vec2 margin;

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

		API_INTERFACE virtual SpColorRGBA* getColor() {
			return color;
		}
		API_INTERFACE virtual void setColor(SpColorRGBA color) {
			this->color->red = color.red;
			this->color->green = color.green;
			this->color->blue = color.blue;
			this->color->alpha = color.alpha;
		}

		API_INTERFACE virtual void setBotomMargin(sp_float bottomMargin)
		{
			sp_assert(bottomMargin > 0.0f, "InvalidArgumentException");
			margin[1] = bottomMargin;
		}
		API_INTERFACE virtual void setLeftMargin(sp_float leftMargin)
		{
			sp_assert(leftMargin > 0.0f, "InvalidArgumentException");
			margin[0] = leftMargin;
		}

		API_INTERFACE virtual ContentAlignment getAlignment()
		{
			return alignment;
		}
		API_INTERFACE virtual void setAlignment(ContentAlignment alignment)
		{
			this->alignment = alignment;
		}

		API_INTERFACE virtual float getBorderWidth()
		{
			return borderWidth;
		}
		API_INTERFACE virtual void setBorderWidth(sp_float borderWidth)
		{
			this->borderWidth = borderWidth;
		}

		API_INTERFACE virtual void setBorderColor(SpColorRGBA color)
		{
			borderColor->red = color.red;
			borderColor->green = color.green;
			borderColor->blue = color.blue;
			borderColor->alpha = color.alpha;
		}
		API_INTERFACE virtual SpColorRGBA* getBorderColor()
		{
			return borderColor;
		}

		API_INTERFACE virtual void renderBorder(SpRenderData renderData, sp_size pointsCount)
		{
			if (borderWidth == 0.0f)
				return;

			glUniform4f(colorLocation, borderColor->red, borderColor->green, borderColor->blue, borderColor->alpha);

			sp_float currentLineWidth = GLConfig::getGLfloat(GL_LINE_WIDTH);
			glLineWidth(borderWidth);

			glDrawArrays(GL_LINE_LOOP, 0, (GLsizei)pointsCount);

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