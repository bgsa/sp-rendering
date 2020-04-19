#include "RendererSettings.h"

namespace NAMESPACE_RENDERING
{
	RendererSettings* instanceRendererSettings;

	RendererSettings::RendererSettings()
	{
	}

	RendererSettings* RendererSettings::getInstance()
	{
		if (instanceRendererSettings == nullptr)
			instanceRendererSettings = new RendererSettings;

		return instanceRendererSettings;
	}

	Rectangle2Df RendererSettings::getScreenBound()
	{
		return Rectangle2Df(
			Vec2f(0.0f, 0.0f),
			Vec2f(float(width), 0.0f),
			Vec2f(float(width), float(height)),
			Vec2f(0.0f, float(height))
		);
	}

	Vec2f& RendererSettings::getRendererPosition()
	{
		return rendererPosition;
	}
	void RendererSettings::setRendererPosition(const Vec2f& position)
	{
		rendererPosition = position;
	}

	sp_float RendererSettings::getWidth()
	{
		return width;
	}
	void RendererSettings::setWidth(sp_float width)
	{
		this->width = width;
	}

	sp_float RendererSettings::getHeight()
	{
		return height;
	}
	void RendererSettings::setHeight(sp_float height)
	{
		this->height = height;
	}

	Vec2f RendererSettings::getSize()
	{
		return Vec2f(width, height);
	}
	void RendererSettings::setSize(sp_float width, sp_float height)
	{
		this->width = width;
		this->height = height;
	}

	float RendererSettings::getAspectRatio()
	{
		return float(getWidth()) / float(getHeight());
	}

	ColorRGBAc RendererSettings::getBackgroudColor()
	{
		return backgroundColor;
	}
	void RendererSettings::setBackgroudColor(ColorRGBAc color)
	{
		this->backgroundColor = color;
	}

	ColorRGBAf RendererSettings::getPixelColor(int xPosition, int yPosition)
	{
		ColorRGBAf pixelColor;

		glReadPixels(xPosition, yPosition, 1, 1, GL_RGBA, GL_FLOAT, &pixelColor);

		return pixelColor;
	}
}