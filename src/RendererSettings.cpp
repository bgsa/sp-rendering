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

	Rectangle2D RendererSettings::getScreenBound()
	{
		return Rectangle2D(
			Vec2(0.0f, 0.0f),
			Vec2(float(width), 0.0f),
			Vec2(float(width), float(height)),
			Vec2(0.0f, float(height))
		);
	}

	Vec2& RendererSettings::getRendererPosition()
	{
		return rendererPosition;
	}
	void RendererSettings::setRendererPosition(const Vec2& position)
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

	Vec2 RendererSettings::getSize()
	{
		return Vec2(width, height);
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

	SpColorRGBA RendererSettings::getBackgroudColor()
	{
		return backgroundColor;
	}
	void RendererSettings::setBackgroudColor(SpColorRGBA color)
	{
		this->backgroundColor = color;
	}

	SpColorRGBA RendererSettings::getPixelColor(int xPosition, int yPosition)
	{
		SpColorRGBA pixelColor;

		glReadPixels(xPosition, yPosition, 1, 1, GL_RGBA, GL_FLOAT, &pixelColor);

		return pixelColor;
	}
}