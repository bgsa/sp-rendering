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

		int width = 0, height = 0;
		Vec2f rendererPosition = Vec2f(0);
		ColorRGBAc backgroundColor = { 128, 128, 128, 255 }; //gray backgtound color

	public:

		API_INTERFACE static RendererSettings* getInstance();

		/// <summary>
		/// Get the position of the renderer on the window/screen
		/// </summary>
		API_INTERFACE Vec2f& getRendererPosition();
		/// <summary>
		/// Set the position of the renderer on the window/screen
		/// </summary>
		void API_INTERFACE setRendererPosition(Vec2f& rendererPosition);

		API_INTERFACE Rectangle2Df getScreenBound();

		int API_INTERFACE getWidth();
		void API_INTERFACE setWidth(int width);

		int API_INTERFACE getHeight();
		void API_INTERFACE setHeight(int height);

		void API_INTERFACE setSize(int width, int height);

		float API_INTERFACE getAspectRatio();

		/// <summary>
		/// Get the default screen backgound color
		/// </summary>
		ColorRGBAc API_INTERFACE getBackgroudColor();

		/// <summary>
		/// Set the default screen backgound color
		/// </summary>
		void API_INTERFACE setBackgroudColor(ColorRGBAc color);

		/// <summary>
		/// Get the color of a screen pixel
		/// </summary>
		ColorRGBAf API_INTERFACE getPixelColor(int xPosition, int yPosition);

	};
}

#endif // !RENDERER_SETTINGS_HEADER