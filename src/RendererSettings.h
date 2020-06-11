#ifndef RENDERER_SETTINGS_HEADER
#define RENDERER_SETTINGS_HEADER

#include "SpectrumRendering.h"
#include <ColorRGBA.h>
#include "Rectangle2D.h"

namespace NAMESPACE_RENDERING
{
	class RendererSettings
	{
	private:
		RendererSettings();

		sp_float width = ZERO_FLOAT, height = ZERO_FLOAT;
		Vec2 rendererPosition = Vec2(ZERO_FLOAT);
		ColorRGBA backgroundColor = { 0.5f, 0.5f, 0.5f, 1.0f }; //gray backgtound color

	public:

		API_INTERFACE static RendererSettings* getInstance();

		/// <summary>
		/// Get the position of the renderer on the window/screen
		/// </summary>
		API_INTERFACE Vec2& getRendererPosition();
		/// <summary>
		/// Set the position of the renderer on the window/screen
		/// </summary>
		API_INTERFACE void setRendererPosition(const Vec2& rendererPosition);

		API_INTERFACE Rectangle2D getScreenBound();

		API_INTERFACE sp_float getWidth();
		API_INTERFACE void setWidth(sp_float width);

		API_INTERFACE sp_float getHeight();
		API_INTERFACE void setHeight(sp_float height);

		API_INTERFACE Vec2 getSize();
		API_INTERFACE void setSize(sp_float width, sp_float height);

		API_INTERFACE sp_float getAspectRatio();

		/// <summary>
		/// Get the default screen backgound color
		/// </summary>
		API_INTERFACE ColorRGBA getBackgroudColor();

		/// <summary>
		/// Set the default screen backgound color
		/// </summary>
		API_INTERFACE void setBackgroudColor(ColorRGBA color);

		/// <summary>
		/// Get the color of a screen pixel
		/// </summary>
		API_INTERFACE ColorRGBA getPixelColor(int xPosition, int yPosition);

	};
}

#endif // !RENDERER_SETTINGS_HEADER